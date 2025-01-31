/*
     FabiWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: tone.cpp - functions for tone feedback and audio transfer + playback
     Audio playback is using wav files, PWM and analog audio amp
     (currently only 22KHz mono 16 bit wav files are supported.)

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "tone.h"
#include <LittleFS.h>

#define SAMPLEPERIOD_uS 38  // fixed sampling rate for 22050 Hz. TBD: improve!
#define SKIP_BYTES 2000     // skip first bytes of a wav file.  TBD: improve!
#define READBUF_LEN 1       // lenght of serial read buffer for audio file transfer

struct sampledata_t {
  File file;
  const int16_t *start;
  const int16_t *p;
  uint32_t count;
  uint32_t len;
  uint8_t isPlaying;
  uint16_t volume;
};
struct sampledata_t sampleData;

struct repeating_timer timer;

/**
   Timer ISR callback function, gets one sample from file and updates PWM
   (disables timer if no sample is left in file)
*/
bool repeating_timer_callback(struct repeating_timer *t) {

  struct sampledata_t *sd = (struct sampledata_t *)(t->user_data);
  //int16_t pwmValue = *(sd->p)++;
  //uint16_t pv = 32767 + pwmValue;
  //uint16_t pv =  pwmValue;
  int16_t pwmValue;

  if (sd->file.available()) {
    sd->file.read((uint8_t *)&pwmValue, 2);  // read signed 16 bit int from file

    if (sd->count>=SKIP_BYTES) {
      analogWrite(AUDIO_SIGNAL_PIN, (pwmValue + 12000) * sd->volume / 100);  // update PWM
      // not sure why an offset of 12000 gives best playback results ....
    }
    sd->count += 2;
   }
   else {
    digitalWrite(AUDIO_AMP_SD_PIN, LOW);
    cancel_repeating_timer(&timer);
    sd->isPlaying = 0;
  }
  return true;
}

/**
   Initialize audio playback (only if AUDIO_SIGNAL_PIN is defined!)
*/
void initAudio() {
  #ifdef AUDIO_SIGNAL_PIN
    pinMode(AUDIO_AMP_SD_PIN, OUTPUT);
    digitalWrite(AUDIO_AMP_SD_PIN, LOW);
    pinMode(AUDIO_SIGNAL_PIN, OUTPUT);
    analogWriteFreq(250000);
    analogWriteResolution(16);
    sampleData.isPlaying = 0;
    sampleData.volume = 100;
  #endif
}

/**
   prepare soundfile path name (include folder prefix or slotname)
*/
void prepSoundFilename(char * extendedFn, char *fn) {
  if ((fn) && (strlen(fn) > 0) && (strlen(fn) < MAX_NAME_LEN-1)) {
    strcpy(extendedFn,SOUND_FOLDER);
    strcat(extendedFn,fn);
  }
  else sprintf(extendedFn,"%sslot%d",SOUND_FOLDER,actSlot);
}

/**
   List available audio files 
*/
void audioList() {
  char path[MAX_PATH_LEN];
  //open current dir
  strcpy(path,SOUND_FOLDER);
  Dir dir = LittleFS.openDir(path);
  //and iterate over files to remove them.
  while(dir.next()) {
    Serial.println(dir.fileName());
  }
  Serial.println(dir.fileName());
}


/**
   Delete audio file 
*/
void audioDelete(char * fn) {
  char soundFilename[MAX_PATH_LEN];
  prepSoundFilename(soundFilename, fn);
  Serial.println("\nRemove Sound File "+String(soundFilename));
  LittleFS.remove(soundFilename);
}

/**
  set audio volume
*/
void audioVolume(uint16_t vol) {
  if ((vol>=0) && (vol <= 200))
    sampleData.volume=vol;
}


/**
   Start an audio playback
*/
void audioPlayback(char * fn) {
  #ifdef AUDIO_SIGNAL_PIN
    char soundFilename[MAX_PATH_LEN];
    prepSoundFilename(soundFilename, fn);
    Serial.println("\nPlay Sound File "+String(soundFilename));
    sampleData.file = LittleFS.open(soundFilename, "r");
    if (sampleData.file.available()) {
      sampleData.count = 0;
      sampleData.isPlaying = 1;
      add_repeating_timer_us(SAMPLEPERIOD_uS, repeating_timer_callback, &sampleData, &timer);
      digitalWrite(AUDIO_AMP_SD_PIN, HIGH);
    } else Serial.printf("cound not open file ...");
  #endif
}


/**
   Receive a wav file transfer from Serial interface.
   A file with name fn is created in the local file system.
   If fn is null, the active slot number is used as file name (with prefix "slot", eg. "slot1")
   The function waits max. 10 seconds for the transfer to start, then reads wav file data (samples) until a timeout of 1 second happens.
*/
uint8_t audioTransfer(char * fn) {
  File f;
  uint32_t cnt = 0, gotBytes=0;
  char soundFilename[MAX_PATH_LEN];
  uint8_t buf[READBUF_LEN];

  prepSoundFilename(soundFilename, fn);
  Serial.print("\nStart upload of SoundFile: ");
  Serial.println(soundFilename);

  uint32_t ts=millis();
  while ((!Serial.available()) && (millis()-ts < WAIT_FOR_TRANSFER_TIMEOUT));
  if (!Serial.available()) return(0);  // if no transfert starts within timeout: abort

  f = LittleFS.open(soundFilename, "w");
  if (f) {
    do {
      gotBytes = Serial.readBytes(buf, READBUF_LEN);   // note that the timeout is used here!
      if (gotBytes > 0) {
        f.write(buf, gotBytes);
        cnt+=gotBytes;
        // Serial.print("*");
      }
    } while (gotBytes == READBUF_LEN);

    // read timeout: transfer finished!
    f.close();
    Serial.println("\nSound File "+String(soundFilename)+" received, " + String(cnt) + " bytes read!");
    sampleData.len=cnt;
    cnt = 0;
  }
  return(1);
}


/**
  Tone Generation using Buzzer 
*/

//  static variables for tone height and pattern
uint16_t toneHeight;
uint16_t toneOnTime;
uint16_t toneOffTime;
uint16_t toneCount = 0;

/**
  updateTones() must be called periodically to handle generation of multiple buzzer tones
*/
void updateTones()
{
  if ((!toneCount) || (!globalSettings.buzzerEnabled)) return;

  static uint16_t toneState = 0;
  static uint16_t cnt = 0;
  uint8_t tonePin = TONE_PIN;

  switch (toneState) {
    case 0:
      tone(tonePin, toneHeight, toneOnTime);
      toneState++;
      break;
    case 1:
      if (++cnt > (toneOnTime + toneOffTime) / UPDATE_INTERVAL )  {
        toneCount--;
        toneState = 0;
        cnt = 0;
      }
      break;
  }
}

/**
  makeTone() creates different tone heights for specific events
*/
void makeTone(uint8_t kind, uint8_t param)
{
  if (!globalSettings.buzzerEnabled) return;

  uint8_t tonePin = TONE_PIN;

  switch (kind) {
    case TONE_ENTER_STRONGPUFF:
      tone(tonePin, 400, 200);
      break;
    case TONE_EXIT_STRONGPUFF:
      tone(tonePin, 400, 100);
      break;
    case TONE_CALIB:
      tone(tonePin, 200, 400);
      break;
    case TONE_CHANGESLOT:
      if (!toneCount) {
        toneHeight = 2000 + 200 * param;
        toneOnTime = 150;
        toneOffTime = 50;
        toneCount = param + 1;
      }
      break;
    case TONE_ENTER_STRONGSIP:
      tone(tonePin, 300, 200);
      break;
    case TONE_EXIT_STRONGSIP:
      tone(tonePin, 300, 100);
      break;
    case TONE_IR:
      tone(tonePin, 2500, 30);
      break;
    case TONE_IR_REC:
      tone(tonePin, 350, 500);
      break;
    case TONE_INDICATE_SIP:
      tone(tonePin, 5000, 5);
      break;
    case TONE_INDICATE_PUFF:
      tone(tonePin, 4000, 5);
      break;
    case TONE_BT_PAIRING:
      tone(tonePin, 230, 4000);
      break;
  }
}
