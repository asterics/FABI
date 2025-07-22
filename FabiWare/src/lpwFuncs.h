/*
     FabiWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

    Module: lpwFuncs.h - Header file for battery and power management functions

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

// Referred datasheet RP2350: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf

#ifndef LPW_FUNCS_H
#define LPW_FUNCS_H

#ifndef FLIPMOUSE   // low power support not available for FlipMouse

#include "hardware/gpio.h"

/**
 * @name PinConfig structure
 * @brief saves the state of the GPIO pins for sleep / dormant mode.
 */
struct PinConfig
{
    gpio_function_t func; // The GPIO function (GPIO_FUNC_SIO, GPIO_FUNC_I2C, etc.)
    bool isOutput;        // Direction: true=output, false=input
    bool pullUp;          // Whether pull-up is enabled
    bool pullDown;        // Whether pull-down is enabled
};

/**
 * @name Time Definitions
 * @brief Constants for inactivity time configurations.
 */
#define inactivityTimeMinutes 1
#define inactivityTimeSeconds 0

#define BATTERY_AVERAGING     4    // number of battery measurements to be averaged  (averaging period see BATTERY_UPDATE_INTERVAL, e.g. every 500ms)

#define MCPSTAT_HIGHZ     0
#define MCPSTAT_LOW       1
#define MCPSTAT_HIGH      2
#define MCPSTAT_UNDEFINED 3
#define CYW43_WL_GPIO_VBUS_PIN 2


/**
 * @name Power supply management functions
 */
bool detectUSB();
void enable3V3();  // enable the 3.3V power rail
void disable3V3(); // disable the 3.3V power rail

/**
 * @name Battery Management Functions
 */
uint8_t batteryPresenceDetector();    // periodically checks battery status, including battery presence
int8_t getBatteryPercentage();        // checks battery-% based on a predetermined interval; presupposes presence of battery
void initBattery();                   // initializes battery-related components
void deinitBattery();                 // deinitializes battery-related components for power saving
uint16_t readPercentage();            // reads and returns mapped battery percentage (0-100%)
void performBatteryManagement();      // must be called periodically (e.g. twice a second) from the main loop

/**
 * @name Power Source Functions (voltage divider)
 */
void enableBattMeasure();             // enables battery voltage measurement circuitry 
void disableBattMeasure();            // disables battery voltage measurement circuitry

/**
 * @name Inactivity Management Functions
 */
void inactivityHandler();                     // prepares sleep mode
void dormantUntilInterrupt(int interruptPin); // puts the device into dormant mode
void userActivity();                          // handles user interaction: resets inactivity counter

/**
 * @name Utility Functions
 */

void reinitAll(); // reinitializes all peripherals and settings after waking up
void readPullState(uint pin, bool &pullUp, bool &pullDown); // reads the pull-up/pull-down state of a given GPIO pin.
void disablePeripherals(); // disables and deinitializes all peripherals
void loadPeripherals();    // restores previously saved peripheral configurations
void savePeripherals();    // saves the current state of peripherals for restoration
void printPeripherals();   // prints the current peripheral configuration (debugging)

#endif // LPW_FUNCS_H
#endif // not FLIPMOUSE
