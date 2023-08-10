/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: sensors.cpp - functions to read pressure & force sensors

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "sensors.h"
#include "modes.h"
#include "utils.h"

/// Enable pin for the MIC5504 LDO for NAU7802 & MPRLS sensors
#define LDO_ENABLE_PIN 7

LoadcellSensor PS;
int sensorWatchdog=-1;

#define MPRLS_READ_TIMEOUT (20)     ///< millis
#define MPRLS_STATUS_POWERED (0x40) ///< Status SPI powered bit
#define MPRLS_STATUS_BUSY (0x20)    ///< Status busy bit
#define MPRLS_STATUS_FAILED (0x04)  ///< Status bit for integrity fail
#define MPRLS_STATUS_MATHSAT (0x01) ///< Status bit for math saturation
#define MPRLS_STATUS_MASK (0b01100101) ///< Sensor status mask: only these bits are set

/**
 * @brief Used pressure sensor type. We can use either the MPXV7007GP
 * sensor connected to an analog pin or the MPRLS sensor board with I2C
 */
typedef enum {MPXV, MPRLS, NO_PRESSURE} pressure_type_t;
pressure_type_t sensor_pressure = NO_PRESSURE;


/**
   @brief Global variables for passing sensor data from the ISR
*/
uint8_t newData = 0;
int32_t mprls_rawval = 512;

/**
   @name initSensors
   @brief initialises I2C interface, prepares NAU and MPRLS readouts. [called from core 1]
   @return none
*/
void initSensors()
{
  //first: switch on LDO for sensors
  pinMode(LDO_ENABLE_PIN,OUTPUT);
  digitalWrite(LDO_ENABLE_PIN,HIGH);
  delay(10);
  
  //detect if there is an MPRLS sensor connected to I2C (Wire)
  Wire1.setClock(400000);  // use 400kHz I2C clock
  Wire1.beginTransmission(MPRLS_ADDR);
  uint8_t result = Wire1.endTransmission();
  //we found the MPRLS sensor, start the initialization
  if (result == 0) {
  
	sensor_pressure = MPRLS;

    // set signal processing parameters for sip/puff (MPRLS pressure sensor)
    PS.setGain(1.0);  // adjust gain for pressure sensor
    PS.enableOvershootCompensation(false);
    PS.setSampleRate(MPRLS_SAMPLINGRATE);
    PS.setMovementThreshold(2500);
    PS.setBaselineLowpass(0.4);
    PS.setNoiseLowpass(10.0);

    // PS.setStartupTime(2000);
    PS.enableAutoCalibration(true);
    PS.setActivityLowpass(1);
    PS.setIdleDetectionPeriod(500);
    PS.setIdleDetectionThreshold(500);
  }

#ifdef DEBUG_OUTPUT_SENSORS
  Serial.println("SEN: Calibrated internal offset");
#endif

}

/**
   @name calibrateSensors
   @brief calibrates the offset values for the sensors (pressure & force)
   @return none
*/
void calibrateSensors()
{
  PS.calib();
}


/**
   @name getMPRLSValue
   @brief called periodically in order to read out MPRLS pressure data
          expected sampling rate ca. 100 Hz
   @param newVal: pointer where result will be stored
   @return status byte of MPRLS
*/
int getMPRLSValue(int32_t * newVal) {
  uint8_t buffer[4]  = {0};

  // request status byte
  // myWire.requestFrom(MPRLS_ADDR,1);
  // buffer[0] = myWire.read();


  //request all 4 bytes
  Wire1.requestFrom(MPRLS_ADDR, 4);
  for (uint8_t i = 0; i < 4; i++) buffer[i] = Wire1.read();

  // update value (ignore status byte errors but return the status byte!)
  *newVal = (uint32_t(buffer[1]) << 16) | (uint32_t(buffer[2]) << 8) | (uint32_t(buffer[3]));

  //trigger new conversion
  Wire1.beginTransmission(MPRLS_ADDR);
  Wire1.write(0xAA);
  Wire1.write(0);
  Wire1.write(0);
  Wire1.endTransmission();

  return (buffer[0]);
}

/**
   @name readPressure
   @brief updates and processes new pressure sensor values form MPRLS. [called from core 1]
   @param data: pointer to I2CSensorValues struct, used by core1
   @return none
*/
void readPressure(struct I2CSensorValues *data)
{
  //static uint32_t ts=0;
  //static int32_t raw_mid=0;
  //int sr=0;
  int actPressure = 512;

  switch (sensor_pressure)
  {
    case MPRLS:
      {

        // get new value from MPRLS chip
        int mprlsStatus = getMPRLSValue(&mprls_rawval);

        // any errors?  - just indicate them via orange LED!
        if (mprlsStatus & MPRLS_STATUS_BUSY) {
          //Serial.println("MPRLS: busy");
          digitalWrite (6, HIGH);
        }
        else if (mprlsStatus & MPRLS_STATUS_FAILED) {
          //Serial.println("MPRLS:failed");
          digitalWrite (6, HIGH);
        }
        else if (mprlsStatus & MPRLS_STATUS_MATHSAT) {
          //Serial.println("MPRLS:saturated");
          digitalWrite (6, HIGH);
        }
        else digitalWrite (6, LOW);

        int med = calculateMedian(mprls_rawval);
        if (abs(med - mprls_rawval) >  SPIKE_DETECTION_THRESHOLD) {
          mprls_rawval = med;
        }

        // calculate filtered pressure value, apply signal conditioning
        int mprls_filtered = PS.process(mprls_rawval);
        if (mprls_filtered > 0) mprls_filtered = sqrt(mprls_filtered);
        if (mprls_filtered < 0) mprls_filtered = -sqrt(-mprls_filtered);

        /*
              // raw_mid=mprls_rawval;
              Serial.print (mprls_rawval-raw_mid); Serial.print(" ");
              Serial.print (mprls_filtered); Serial.print(" ");
              sr= 1000000 / (micros()-ts);
              ts=micros();
              Serial.print (sr); Serial.print(" ");
              Serial.println(" ");
        */

        actPressure = 512 + mprls_filtered / MPRLS_DIVIDER;

        // clamp to 1/1022 (allows disabling strong sip/puff)
        if (actPressure < 1) actPressure = 1;
        if (actPressure > 1022) actPressure = 1022;

        // during calibration period: set pressure to center (bypass)
        if (data->calib_now) actPressure = 512;
      }
      break;

    case NO_PRESSURE:
    default:
      actPressure = 512;
      break;
  }
  //reset watchdog
  sensorWatchdog = 0;
  // here we provide new pressure values for further processing by core 0 !
  mutex_enter_blocking(&(data->sensorDataMutex));
  data->pressure = actPressure;
  mutex_exit(&(data->sensorDataMutex));
}


/**
   @name checkSensorWatchdog
   @brief checks if an integer value which should be periodically reset when I2C-sensordata is ready exceeds a certain value
   @return true: value within normal range  false: value exceeded -> action must be taken
*/
uint8_t checkSensorWatchdog() {
  //if we never received any valid values, proceed
  if (sensorWatchdog == -1) return (true);
  //if we received at least one time values, but then it stops,
  //check the value and reset after ~1s (SENSOR_WATCHDOG_TIMEOUT)
  if (sensorWatchdog++ > SENSOR_WATCHDOG_TIMEOUT)
    return (false);
  return (true);
}

/**
   @name swap
   @brief swaps two integers, helper function for median
*/
void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}


/**
   @name quick sort
   @brief sorts array of integers, helper function for median
*/
void quickSort(int values[], int left, int right) {
  if (left >= right) {
    return;
  }

  int pivot = values[left];
  int i = left + 1;
  int j = right;

  while (1) {
    while (i <= j && values[i] <= pivot) i++;
    while (j >= i && values[j] >= pivot) j--;
    if (i > j)
      break;

    swap(&values[i], &values[j]);
  }
  swap(&values[left], &values[j]);

  quickSort(values, left, j - 1);
  quickSort(values, j + 1, right);
}


/**
   @name calculateMedian
   @brief calculates median value (attention: uses static buffer, useable for just 1 signal!)
   @param value: next sample
   @return median value
*/
int calculateMedian(int value) {
  static int medianBuf[MEDIAN_VALUES] = {0};
  static int medianBufPos = 0;
  int values[MEDIAN_VALUES];

  medianBuf[medianBufPos] = value;
  medianBufPos++;
  if (medianBufPos >= MEDIAN_VALUES) medianBufPos = 0;
  memcpy(values, medianBuf, sizeof(int)*MEDIAN_VALUES);

  quickSort(values, 0, MEDIAN_VALUES - 1);

  if (MEDIAN_VALUES % 2 == 0) {
    int midIndex1 = MEDIAN_VALUES / 2 - 1;
    int midIndex2 = MEDIAN_VALUES / 2;
    return (values[midIndex1] + values[midIndex2]) / 2;
  } else {
    int midIndex = MEDIAN_VALUES / 2;
    return values[midIndex];
  }
}
