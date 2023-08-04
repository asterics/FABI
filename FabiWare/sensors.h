/*
     FabiWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: sensors.cpp - functions to read pressure & force sensors

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _SENSORS_H_
#define _SENSORS_H_

#include "FlipWare.h"        // we need the SensorData and SlotSettings structure definitions
#include "Wire.h"            // MPRLS pressure sensor and NAU7802 sensor use I2C
#include <LoadcellSensor.h>  // for signal conditioning

#define MPRLS_ADDR 0x18          // I2C address of the MPRLS pressure sensor 

#define MPRLS_DIVIDER 2                 // divider for the MPRLS raw value.
#define MPRLS_SAMPLINGRATE   100        // sampling frequency of MPRLS sensor
#define MEDIAN_VALUES 5                 // number of values used for median-based spike filter (for MPRLS sensor)
#define SPIKE_DETECTION_THRESHOLD 1000  // distance from median value which classifies a spike

/**
   @brief Sensorboard IDs for different signal processing parameters
*/
#define SENSORBOARD_SENSITIVITY_HIGH     0
#define SENSORBOARD_SENSITIVITY_MEDIUM   1
#define SENSORBOARD_SENSITIVITY_LOW      2
#define SENSORBOARD_SENSITIVITY_VERY_LOW 3
#define SENSORBOARD_NODMS_HIGH           4
#define SENSORBOARD_NODMS_MEDIUM         5
#define SENSORBOARD_NODMS_LOW            6
#define SENSORBOARD_REPORT_X             10   // enable / disable signal processing values reporting for X axis
#define SENSORBOARD_REPORT_Y             11   // enable / disable signal processing values reporting for Y axis

/**
   @name initSensors
   @brief initializes the sensors (pressure & force)
   @return none
*/
void initSensors();

/**
   @name calibrateSensors
   @brief calibrates the offset values for the sensors (pressure & force)
   @return none
*/
void calibrateSensors();

/**
   @name readPressure
   @brief read current pressure sensor (either MPXV7007GP or MPRS)
   @note For the MPRLS sensor, it returns the previous measurement & triggers a new one!
   @return none
*/
void readPressure(struct I2CSensorValues *data);

/**
   @name checkSensorWatchdog
   @brief checks if an integer value which should be periodically reset when I2C-sensordata is ready exceeds a certain value
   @return true: value within normal range  false: value exceeded -> action must be taken
*/
uint8_t checkSensorWatchdog();


/**
   @name getMPRLSValue
   @brief called periodically in order to read out MPRLS pressure data
          expected sampling rate ca. 100 Hz
   @param newVal: pointer where result will be stored
   @return status byte of MPRLS
*/
int getMPRLSValue(int32_t * newVal);


/**
   @name calculateMedian
   @brief calculates median value (attention: uses static buffer, useable for just 1 signal!)
   @param value: next sample
   @return median value
*/
int calculateMedian(int value);


#endif /* _SENSORS_H_ */
