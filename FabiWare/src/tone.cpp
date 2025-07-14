/*print
     FabiWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: tone.cpp - functions for tone feedback (using a piezo buzzer) and audio transfer + playback
     Audio playback is using wav files, PWM and analog audio amp
     (currently only 22KHz mono 16 bit wav files are supported. TBD: mp3 support?)

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "tone.h"
#include <LittleFS.h>
#include "hardware/pwm.h"
#include <hardware/clocks.h>

// constant definitions for audio playback
#define SAMPLEPERIOD_uS 38  // fixed sampling rate for 22050 Hz. TBD: improve!
#define SKIP_BYTES 2000     // skip first bytes of a wav file.  TBD: improve!
#define READBUF_LEN 1       // lenght of serial read buffer for audio file transfers


// data structure for audio playback (wav file and play status)
struct sampledata_t {
  File file;
  struct repeating_timer audioTimer; // the timer for audio sample callbacks
  const int16_t *start;
  const int16_t *p;
  uint32_t count;
  uint32_t len;
  uint8_t isPlaying;
};
struct sampledata_t sampleData;

/**
   @name audio_timer_callback
   @param repeating_timer: the repeating timer data
   @brief periodic alarm callback function, called with sampling frequency when audio playback is active.
   Gets one sample from file and updates PWM (disables timer alarm if no sample is left in file)
   @return none
*/
bool audio_timer_callback(struct repeating_timer *t) {

  struct sampledata_t *sd = (struct sampledata_t *)(t->user_data);
  //int16_t pwmValue = *(sd->p)++;
  //uint16_t pv = 32767 + pwmValue;
  //uint16_t pv =  pwmValue;
  int16_t pwmValue;

  if (sd->file.available()) {
    sd->file.read((uint8_t *)&pwmValue, 2);  // read signed 16 bit int from file
    if (sd->count>=SKIP_BYTES) {
      analogWrite(AUDIO_SIGNAL_PIN, (pwmValue + 12000) * globalSettings.audioVolume / 100);  // update PWM
      // not sure why an offset of 12000 gives best playback results ....
    }
    sd->count += 2;
   }
   else {
    digitalWrite(AUDIO_AMP_SD_PIN, LOW);  // turn off audio amp
    if (!cancel_repeating_timer(&(sd->audioTimer))) 
      Serial.println("Audio: could not cancel Timer!");
    sd->file.close();
    sd->isPlaying = 0;
  }
  return true;
}

/**
   @name initAudio
   @brief Initialize audio playback (only if AUDIO_SIGNAL_PIN is defined!)
   @return none
*/
void initAudio() {
  #ifdef AUDIO_SIGNAL_PIN
    pinMode(AUDIO_AMP_SD_PIN, OUTPUT);
    digitalWrite(AUDIO_AMP_SD_PIN, LOW);
    pinMode(AUDIO_SIGNAL_PIN, OUTPUT);
  	// Note: analogWriteFreq() and Range must be set when playing audio file
    // (different settings are needed for IR code playback, see infrared.cpp)
    sampleData.isPlaying = 0;
  #endif
}

/**
   @name prepSoundFilename
   @param extendedFn: target char buffer where filename including path is stored (enough memory must be reserverd!)
   @param fn: filename
   @brief  prepare soundfile path name (prepend folder name). 
   if no filename was given as parameter, a default name for the active slot is created, e.g. "slot0"
   @return none
*/
void prepSoundFilename(char * extendedFn, char *fn) {
  if ((fn) && (strlen(fn) > 0) && (strlen(fn) < MAX_NAME_LEN-1)) {
    strcpy(extendedFn,SOUND_FOLDER);
    strcat(extendedFn,fn);
  }
  else sprintf(extendedFn,"%sslot%d",SOUND_FOLDER,actSlot);
}

/**
   @name prepSoundFilenameSlotNum
   @param extendedFn: target char buffer where filename including path is stored (enough memory must be reserverd!)
   @param slot: slotnumber
   @brief  prepare soundfile path name for a given slot number
   @return none
*/
void prepSoundFilenameSlotnum(char * extendedFn, int slot) {
  sprintf(extendedFn,"%sslot%d",SOUND_FOLDER,slot);
}

/**
   @name audioList
   @brief Print a list of available audio files 
   @return none
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
  Serial.println("OK");
}

/**
   @name audioRemove
   @param fn: filename of wav file in LitteFS
   @brief removes the file from LitteFS
   @return none
*/
void audioRemove(char * fn) {
  char soundFilename[MAX_PATH_LEN];
  prepSoundFilename(soundFilename, fn);
  #ifdef DEBUG_OUTPUT_FULL
    Serial.println("\nRemove Sound File "+String(soundFilename));
  #endif
  LittleFS.remove(soundFilename);
}

/**
   @name audioPlayback
   @param fn: filename of wav file in LitteFS
   @brief Start an audio playback
   @return none
*/void audioPlayback(char * fn) {
  #ifdef AUDIO_SIGNAL_PIN
    if ((sampleData.isPlaying) || (globalSettings.audioVolume==0)) return;
    char soundFilename[MAX_PATH_LEN];
    prepSoundFilename(soundFilename, fn);
    #ifdef DEBUG_OUTPUT_FULL
      Serial.println("\nPlay Sound File "+String(soundFilename));
    #endif
    sampleData.file = LittleFS.open(soundFilename, "r");
    if (sampleData.file.available()) {
      analogWriteFreq(120000);    // 120KHz PWM frequency for audio playback
      analogWriteResolution(16);  // 16 bit resolution (effectively: 10 bits due to scaling)
      sampleData.count = 0;
      sampleData.isPlaying = 1;
      int r=alarm_pool_add_repeating_timer_us(app_alarm_pool, SAMPLEPERIOD_uS, audio_timer_callback, &sampleData, &(sampleData.audioTimer));
      if (!r) Serial.println("Audio: no Timer available!");
      else digitalWrite(AUDIO_AMP_SD_PIN, HIGH);  // turn no audio amp
    } else {
      #ifdef DEBUG_OUTPUT_FULL
        Serial.printf("cound not open file ...");
      #endif
    }
  #endif
}

/**
   @name audioTransfer
   @param fn: filename to store the audio/wav data in LitteFS
   @brief  Receive a wav file transfer from Serial interface.
   A file with name fn is created in the local file system.
   If fn is null, the active slot number is used as file name (with prefix "slot", eg. "slot1")
   The function waits max. 10 seconds for the transfer to start, then reads wav file data (samples) until a timeout of 1 second happens.
   @return none
*/
uint8_t audioTransfer(char * fn) {
  File f;
  uint32_t cnt = 0, gotBytes=0;
  char soundFilename[MAX_PATH_LEN];
  uint8_t buf[READBUF_LEN];

  prepSoundFilename(soundFilename, fn);
  #ifdef DEBUG_OUTPUT_FULL
    Serial.print("\nStart upload of SoundFile: ");
    Serial.println(soundFilename);
  #endif

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
    #ifdef DEBUG_OUTPUT_FULL
      Serial.println("\nSound File "+String(soundFilename)+" received, " + String(cnt) + " bytes read!");
    #endif
    sampleData.len=cnt;
    cnt = 0;
  }
  return(1);
}


/**
  Tone Generation using Buzzer 
*/

//  static variables for tone height and pattern
uint16_t freqtable[] = {523, 587, 659, 698, 784, 880, 988, 1047};    // one octave for chromatic tone feedback
uint16_t toneHeight;
uint16_t toneOnTime;
uint16_t toneOffTime;
uint16_t toneCount = 0;

/**
   @name setup_pwm
   @param gpio: pin where tone signal is put out
   @param frequency: desired tone frequency
   @param wrap_value: maximum PWM value (higher values allow lower frequencies)
   @brief Configure the PWM on the given GPIO to produce a tone at 'frequency' (Hz). Duty cycle is set to 50%.
   @return none

*/
void setup_pwm(uint gpio, uint frequency, uint16_t wrap_value = 10000) {
    // Set the GPIO function to PWM
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    
    // Get the PWM slice and channel for the given GPIO
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    uint channel = pwm_gpio_to_channel(gpio);
    
    // Get the current system clock frequency (usually 125 MHz)
    uint32_t sys_clock = clock_get_hz(clk_sys);
    
    // Calculate the clock divider
    // divider = sys_clock / (frequency * (wrap + 1))
    float divider = (float)sys_clock / (frequency * (wrap_value + 1));
    
    // Configure the PWM with the new divider and wrap value
    pwm_set_clkdiv(slice_num, divider);
    pwm_set_wrap(slice_num, wrap_value);
    
    // Set duty cycle to 50% (wrap_value/2) for a square wave
    pwm_set_chan_level(slice_num, channel, wrap_value / 2);
    
    // Enable the PWM slice
    pwm_set_enabled(slice_num, true);
}

/**
   @name stop_pwm
   @param gpio: pin where tone signal is put out
   @brief stops a running tone pwm
   @return none
*/
void stop_pwm(uint gpio) {
    pwm_set_enabled(pwm_gpio_to_slice_num(gpio), false);
    pinMode (gpio,OUTPUT);
    digitalWrite(gpio,LOW);
}

/**
   @name stop_pwm_callback
   @brief stops a running tone pwm via an alarm callback after the tone duration
   @return the return value determines the alarm behaviour: 0 no further alarm is scheduled
*/
int64_t stop_pwm_callback(alarm_id_t id, void *user_data)
{
  stop_pwm(TONE_PIN);
  return (0);  // no further callbacks
}

void pwmTone(int tp, int f, int dur) {
    // Initialize serial, BLE, etc.
    setup_pwm(tp, f);  // Start tone pwm on given gpio pin
    if(alarm_pool_add_alarm_in_us(app_alarm_pool, dur*1000, stop_pwm_callback, nullptr, true) <0)
    { Serial.println("TONE: no alarm available!");}

}

/**
   @name updateTones()
   @brief must be called periodically to handle generation of multiple buzzer tones (for indicating active slot number)
   @return none
*/
void updateTones()
{
  if ((!toneCount) || (!globalSettings.buzzerMode)) return;

  static uint16_t toneState = 0;
  static uint16_t cnt = 0;
  
  switch (toneState) {
    case 0:
      pwmTone(TONE_PIN, toneHeight, toneOnTime);
      toneState++;
      break;
    case 1:
      if (++cnt > (toneOnTime + toneOffTime) / UPDATE_INTERVAL )  {
        toneCount--;
        toneState = 0;
        cnt = 0;
        // noTone(TONE_PIN);
      }
      break;
  }
}

/**
   @name makeTone()
   @param kind: id for tone type
   @param slotNumber: the active slot (for indicating the number acoustically)
   @brief creates different tone heights for specific events
   @return none
*/

void makeTone(uint8_t kind, uint8_t slotNumber)
{
  if (!globalSettings.buzzerMode) return;

  switch (kind) {
    case TONE_STARTUP:
      pwmTone(TONE_PIN,freqtable[0],180);  delay(200);
      pwmTone(TONE_PIN,freqtable[2],180);  delay(200);
      pwmTone(TONE_PIN,freqtable[4],100);  delay(100);
      break;

    case TONE_ENTER_STRONGPUFF:
      pwmTone(TONE_PIN, 400, 200);
      break;
    case TONE_EXIT_STRONGPUFF:
      pwmTone(TONE_PIN, 400, 100);
      break;
    case TONE_CALIB:
      pwmTone(TONE_PIN, 200, 400);
      break;
    case TONE_CHANGESLOT:
      if (!toneCount) {
        toneHeight = freqtable[slotNumber%8];  // choose tone from chromatic scale
        toneOnTime = 150;
        toneOffTime = 50;
        if (globalSettings.buzzerMode == 2) toneCount = slotNumber + 1;
        else toneCount = 1;
      }
      break;
    case TONE_ENTER_STRONGSIP:
      pwmTone(TONE_PIN, 300, 200);
      break;
    case TONE_EXIT_STRONGSIP:
      pwmTone(TONE_PIN, 300, 100);
      break;
    case TONE_IR:
      pwmTone(TONE_PIN, 2500, 30);
      break;
    case TONE_IR_REC:
      pwmTone(TONE_PIN, 350, 500);
      break;
    case TONE_INDICATE_SIP:
      pwmTone(TONE_PIN, 5000, 5);
      break;
    case TONE_INDICATE_PUFF:
      pwmTone(TONE_PIN, 4000, 5);
      break;
    case TONE_BT_PAIRING:
      pwmTone(TONE_PIN, 230, 4000);
      break;
  }
}


/* maybe decode MP3 onboard?

#include "helix/pub/mp3dec.h"
#define MP3_BUFSIZE 1024

MP3Decoder mp3;
MP3FrameInfo mp3FrameInfo;

void playMp3() {

   File mp3File = LittleFS.open("/audio.mp3", "r");
    if (!mp3File) {
        Serial.println("Error opening MP3 file!");
        return;
    }

    mp3.Begin();
    uint8_t mp3_buffer[MP3_BUFSIZE];
    size_t bytesRead = mp3File.read(mp3_buffer, sizeof(mp3_buffer));

    // Decode first frame to get sample rate info
    int decodedSamples = mp3.Decode(mp3_buffer, bytesRead, nullptr);
    if (decodedSamples > 0) {
        mp3.GetLastFrameInfo(&mp3FrameInfo);
        playAudio(pcm_buffer, decodedSamples, mp3FrameInfo.samprate);
    } else {
        Serial.println("Failed to decode MP3 file.");
    }

    mp3.End();
    mp3File.close();

void playAudio(short *pcm, int samples, int sr) {
    for (int i = 0; i < samples; i++) {
        analogWrite(AUDIO_OUT_PIN, map(pcm[i], -32768, 32767, 0, 255));
        delayMicroseconds(1000000/sr);
    }
}
*/