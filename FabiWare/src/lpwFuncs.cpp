/*
     FabiWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

    Module: lpwFuncs.cpp - C module for battery and power management functions

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifdef RP2350   // low power support only available for RP2350

#include "FlipWare.h"
#include "gpio.h"
#include "display.h"
#include "pico/cyw43_arch.h"

extern "C" {
  #include "../lib/power/sleep.h"
}

unsigned long inactivityTime=0;  // measures user inactivity (in ms)
struct PinConfig pinBackup[29];  // array to save pin configurations

/**
 * @name detectUSB
 * @brief Reads the state of defined CYW43 VBUS Pin (2)
 * @return true : USB Connected (VBUS Presence detected)
 *         false : USB Disconnected (VBUS Presence not detected)
 */
bool detectUSB(){
  sensorData.usbConnected = cyw43_arch_gpio_get(CYW43_WL_GPIO_VBUS_PIN);
  static bool prevValue = !sensorData.usbConnected;
  
  if (prevValue != sensorData.usbConnected){
    #ifdef DEBUG_BATTERY_MANAGEMENT
     if (sensorData.usbConnected) Serial.println("USB Connected");
     else Serial.println("USB Disconnected");  // note that this only makes sense if Serial connected still present!
    #endif
    prevValue = sensorData.usbConnected;
  }
  return sensorData.usbConnected;
}

/**
 * @name batteryPresenceDetector
 * @brief Uses a state-machine to check for the battery's presence.
 *        There are three modes that need to be differentiated: 
 *          a. HIGH Z : Shutdown, No battery present 
 *          b. LOW : Charging (Preconditioning, Constant-Current Fast Charge, Constant Voltage) 
 *          c. HIGH : Charge Complete, Standby
 * @return MCPSTAT_HIGHZ : No battery is present / MCP is in shutdown.
 *         MCPSTAT_LOW   : The battery is present, preconditioning / charging.
 *         MCPSTAT_HIGH  : The battery is present, fully charged - standby mode.
 *         MCPSTAT_UNDEFINED  : Do nothing - default return value when state is being determined.
 */
uint8_t batteryPresenceDetector(){
  static uint16_t battStateCounter=0;  // determines when to pull up/down, to read
  static uint16_t  battStateSum=0;  // accumulates digital reads of the MCP Stat pin which eitehr floats, is LOW or HIGH.
  static uint8_t state = MCPSTAT_UNDEFINED; 

  switch (battStateCounter++) {
    case 0:
      battStateSum=0;  // reset accumulator variable
      gpio_disable_pulls(CHARGE_STAT_PIN); gpio_pull_up(CHARGE_STAT_PIN); break;
    case 1:
      battStateSum += gpio_get(CHARGE_STAT_PIN); break;
    case 2:
      gpio_disable_pulls(CHARGE_STAT_PIN); gpio_pull_down(CHARGE_STAT_PIN); break;
    case 3:
      battStateSum += gpio_get(CHARGE_STAT_PIN); 

      // now we can determine the battery state:
      if (battStateSum == 0) {                           // battery is charging
        state = MCPSTAT_LOW;
        #ifdef DEBUG_BATTERY_MANAGEMENT
          Serial.println("LOW   :\tBattery is Charging"); 
        #endif
      }
      else if (battStateSum == 2) {                    // charging complete or standby
        state = MCPSTAT_HIGH;
        #ifdef DEBUG_BATTERY_MANAGEMENT
          Serial.println("HIGH  :\tBattery Charge Completed"); 
        #endif
      } 
      else {
        state = MCPSTAT_HIGHZ;     // shutdown or no battery present
        #ifdef DEBUG_BATTERY_MANAGEMENT
          Serial.println("HIGH Z:\tBattery not present or in shutdown Mode");
        #endif
      }
      battStateCounter=0; // reset state counter!
    break;
  }

  sensorData.MCPSTAT=state;
  return state;
}

/**
 * @name getBatteryPercentage
 * @brief returns an averages battery level (averaging count is defined by the constant BATTERY_AVERAGING).
 * @return 0 - 100  : The average of the battery-% read over last n reads.
 *         -1       : Undefined battery level (no battery present / MCP Stat is HighZ).
 */
int8_t getBatteryPercentage(){
  static int battSum=0, battReadCounter=0, result=0;
  if(sensorData.MCPSTAT == MCPSTAT_HIGHZ) return (-1);
  battReadCounter++;
  battSum += readPercentage();
  if (battReadCounter >= BATTERY_AVERAGING ){ 
    result = battSum / BATTERY_AVERAGING;
    battSum = 0; battReadCounter = 0;
  }
  return result;
}

/**
 * @name readPercentage
 * @brief Reads and calculates the battery percentage using ADC input.
 * @return uint16_t Battery percentage (0-100%).
 */
uint16_t readPercentage() {
  int16_t value = map(analogRead(V_BATT_MEASURE_PIN), 518, 682, 0, 100);   // map ADC value to battery percentage range
  return constrain(value, 0, 100); 
}

/**
 * @name performBatteryManagement
 * @brief called periodically, to update battery status
 * @return none
 */
void performBatteryManagement()  {
  detectUSB();
  batteryPresenceDetector();
  sensorData.currentBattPercent = getBatteryPercentage();
  batteryDisplay();
  #ifdef DEBUG_BATTERY_MANAGEMENT
    Serial.println("Battery level="+String(sensorData.currentBattPercent));
  #endif

  // check user inactivity, possibly initiate power save mode
  if (!sensorData.usbConnected) {
    inactivityTime += BATTERY_UPDATE_INTERVAL;
    if (inactivityTime >= (inactivityTimeMinutes*60000 + inactivityTimeSeconds*1000))
      inactivityHandler();  // time to go to sleep...
  } else inactivityTime=0;
}

/**
 * @name enable3V3
 * @brief Enables the 3.3V power rail for peripherals like LCD, Neopixel or external sensors
 */
void enable3V3() {
  gpio_init(LDO_ENABLE_PIN);
  gpio_set_dir(LDO_ENABLE_PIN, true);
  gpio_put(LDO_ENABLE_PIN, true);
}

/**
 * @name disable3V3
 * @brief Disables the 3.3V power rail
 */
void disable3V3() {
  gpio_put(LDO_ENABLE_PIN, false);
  gpio_set_dir(LDO_ENABLE_PIN, false);
  // gpio_deinit(LDO_ENABLE_PIN);
}

/**
 * @name initBattery
 * @brief Initializes battery-related power-saving mechanisms and peripherals.
 */
void initBattery() {
  detectUSB();
  gpio_init(CHARGE_STAT_PIN);
  enableBattMeasure();
}

/**
 * @name deinitBattery
 * @brief Deinitializes battery-related peripherals.
 */
void deinitBattery() {
  gpio_deinit(CHARGE_STAT_PIN);
  disableBattMeasure();
}

/**
 * @name disableBattMeasure
 * @brief Disables the battery measurement circuitry.
 */
void disableBattMeasure() {
  gpio_set_dir(V_BATT_MEASURE_PIN, false);
  gpio_deinit(V_BATT_MEASURE_PIN);
  gpio_disable_pulls(V_BATT_MEASURE_PIN);
}

/**
 * @name enableBattMeasure
 * @brief Enables the battery measurement circuitry.
 */
void enableBattMeasure() {
  gpio_init(CHARGE_STAT_PIN);
  gpio_init(V_BATT_VD_SWITCH_PIN);
  gpio_set_dir(V_BATT_VD_SWITCH_PIN, false);
  gpio_init(V_BATT_MEASURE_PIN);
  gpio_set_dir(V_BATT_MEASURE_PIN, false);
}

/**
 * @name dormantUntilInterrupt
 * @brief Puts the device into dormant mode until a specified GPIO interrupt wakes it up.
 * @param interruptPin GPIO pin to monitor for the interrupt.
 */
void dormantUntilInterrupt(int interruptPin) {
  delay(1); // small delay to ensure system stability, might be redundant
  sleep_run_from_lposc(); // use low-power oscillator for minimal power consumption
  sleep_goto_dormant_until_edge_high(interruptPin); // wait for rising edge interrupt
  sleep_power_up(); // restore sys clocks after waking up (using rosc -> jump starts processor)
  delay(400); // allow some time for system to stabilize after restoring sys clocks
}

/**
 * @name inactivityHandler
 * @brief Handles inactivity by transitioning the system to dormant mode.
 */
void inactivityHandler() {
  inactivityTime=0;
  deinitBattery();
  #ifndef FLIPMOUSE
    MouseBLE.end();     // turn off Bluetooth
  #endif
  #ifdef DEBUG_BATTERY_MANAGEMENT
    Serial.println("goodbye, going to sleep!");
  #endif 
  Serial.flush();
  Serial.end();
  displayMessage((char*)"ByeBye");
  delay(2000);   // time for the user to read the message
  disable3V3();  // shut down peripherals
  digitalWrite(LED_BUILTIN,LOW);  // make sure the internal LED is off

  dormantUntilInterrupt(input_map[0]); // enter sleepMode, use Button1 to wakeup!
  //  <--   now sleeping!  
  
  watchdog_reboot(0, 0, 10);  // cause a watchdog reset to wake everything up!
  while (1) { continue; }     

  /*
    // Note: this soft startup did not work ... TBD!
    Serial.begin(115200);
    delay(3000);  // allow some time for serial interface to come up
    enable3V3();
    initBattery();
    displayReinit();
    initButtons();
    initDebouncers();
  */
}

/**
 * @name userActivity
 * @brief Resets the inactivity counter upon user interaction.
 */
void userActivity() { // Call of this function can be found in line 181, buttons.cpp
  inactivityTime=0;   // reset the inactivity counter!
}


/**
 * @name savePeripherals
 * @brief Saves the current GPIO configuration for all pins.
 */
void savePeripherals() {
  for (uint pin = 0; pin < 29; pin++) {
    if (pin == 23 || pin == 24 || pin == 25) continue; // skip specific pins
    bool pullUp, pullDown;
    pinBackup[pin].func = gpio_get_function(pin);
    pinBackup[pin].isOutput = (gpio_get_dir(pin) == GPIO_OUT);
    readPullState(pin, pullUp, pullDown);
    pinBackup[pin].pullUp = pullUp;
    pinBackup[pin].pullDown = pullDown;
  }
  Serial.println("Saved SIO pin configs.");
}

/**
 * @name disablePeripherals
 * @brief Disables all GPIO peripherals by deinitializing the pins.
 */
void disablePeripherals() {
  for (uint pin = 0; pin < 29; pin++) {
    if (pin == 23 || pin == 24 || pin == 25) continue; // skip specific pins
    if (pinBackup[pin].func == GPIO_FUNC_SIO) {
      gpio_disable_pulls(pin);
      gpio_deinit(pin);
    }
  }
}

/**
 * @name printPeripherals
 * @brief Prints the current GPIO configuration to the Serial monitor.
 */
void printPeripherals() {
  for (int pin = 0; pin < 29; pin++) {
    if (pin == 23 || pin == 24 || pin == 25) continue;
    Serial.print(pin);
    switch (pinBackup[pin].func) {
      case GPIO_FUNC_HSTX: Serial.print(": HSTX"); break;
      case GPIO_FUNC_SPI: Serial.print(": SPI"); break;
      case GPIO_FUNC_UART: Serial.print(": UART"); break;
      case GPIO_FUNC_I2C: Serial.print(": I2C"); break;
      case GPIO_FUNC_PWM: Serial.print(": PWM"); break;
      case GPIO_FUNC_SIO: Serial.print(": SIO"); break;
      case GPIO_FUNC_PIO0: Serial.print(": PIO0"); break;
      case GPIO_FUNC_PIO1: Serial.print(": PIO1"); break;
      case GPIO_FUNC_PIO2: Serial.print(": PIO2"); break;
      default: Serial.print(": NULL"); break;
    }
    Serial.print(", ");
    Serial.print(pinBackup[pin].isOutput ? "OUT" : "IN");
    if (pinBackup[pin].pullUp) Serial.print(", pullUp");
    if (pinBackup[pin].pullDown) Serial.print(", pullDown");
    Serial.println();
  }
}

/**
 * @name loadPeripherals
 * @brief Restores the saved GPIO configuration for all pins.
 */
void loadPeripherals() {
  for (uint pin = 0; pin < 29; pin++) {
    if (pin == 23 || pin == 24 || pin == 25) continue;
    if (pinBackup[pin].func == GPIO_FUNC_SIO) {
      gpio_init(pin);
      gpio_set_function(pin, pinBackup[pin].func);
      gpio_set_dir(pin, pinBackup[pin].isOutput);
      if (pinBackup[pin].pullUp) gpio_pull_up(pin);
      if (pinBackup[pin].pullDown) gpio_pull_down(pin);
    }
  }
}

// #define DORMANT_SOURCE_LPOSC 1
// void goSleep(uint gpio_pin, bool edge, bool high){
//   uint src_hz = 32 * KHZ;
//   uint clk_ref_src = CLOCKS_CLK_REF_CTRL_SRC_VALUE_LPOSC_CLKSRC;
//   clock_configure(clk_ref, clk_ref_src, 0, src_hz, src_hz);
//   clock_configure(clk_sys, CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLK_REF, 0, src_hz, src_hz);
//   clock_configure(clk_peri, 0, CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS, src_hz, src_hz);

//   clock_stop(clk_adc);
//   clock_stop(clk_usb);
//   clock_stop(clk_hstx);
//   pll_deinit(pll_sys);
//   pll_deinit(pll_usb);

//   // // Assuming both xosc and rosc are running at the moment
//   //   if (dormant_source == DORMANT_SOURCE_XOSC) {
//   //       // Can disable rosc
//   //       rosc_disable();
//   //   } else {
//   //       // Can disable xosc
//   //       xosc_disable();
//   //   }

//   // the lines up above assume RP2040 that has no LPOSC so it always disables xosc when
//   // there's a will to use LPOSC as dormant source:
//   xosc_disable();

//   bool low = !high;
//   bool level = !edge;

//   uint32_t event = 0

//   if (level && low) event = IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_LEVEL_LOW_BITS;
//   if (level && high) event = IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_LEVEL_HIGH_BITS;
//   if (edge && high) event = IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_EDGE_HIGH_BITS;
//   if (edge && low) event = IO_BANK0_DORMANT_WAKE_INTE0_GPIO0_EDGE_LOW_BITS;

//   gpio_init(gpio_pin);
//   gpio_set_input_enabled(gpio_pin, true);
//   gpio_set_input_hysteresis_enabled(gpio_pin, true);
//   gpio_set_dormant_irq_enabled(gpio_pin, event, true);

//   // here belongs going dormant()

//   /** @note when using crystal oscillator as dormant source */
//   // xosc_dormant();

//   /** @note when using ring oscillator as dormant source */
//   // hw_clear_bits(&rosc_hw->status, ROSC_STATUS_BADWRITE_BITS);

//   // if !(rosc_hw->status & ROSC_STATUS_BADWRITE_BITS){
//   //   rosc_hw->dormant = ROSC_DORMANT_VALUE_DORMANT;

//   //   if !(rosc_hw->status & ROSC_STATUS_BADWRITE_BITS){
//   //     // all good.
//   //   } else {
//   //     // something went wrong.
//   //   }
//   //   // wait for ring oscillator to become stable once woken up
//   //   while(!(rosc_hw->status & ROSC_STATUS_STABLE_BITS));
//   // } else {
//   //   // something went wrong.
//   // }

//   // EXECUTION WILL STOP HERE.

//   gpio_acknowledge_irq(gpio_pin, event);
//   gpio_set_input_enabled(gpio_pin, false);

  
//   // To be called after waking up from sleep/dormant mode to restore system clocks properly
//   // if !(rosc_hw->status & ROSC_STATUS_BADWRITE_BITS){
//   //   rosc_hw->ctrl = ROSC_CTRL_ENABLE_BITS;

//   //   if !(rosc_hw->status & ROSC_STATUS_BADWRITE_BITS){
//   //     // all good.
//   //   } else {
//   //     // something went wrong.
//   //   }
//   //   // wait for ring oscillator to become stable once woken up
//   //   while(!(rosc_hw->status & ROSC_STATUS_STABLE_BITS));
//   // } else {
//   //   // something went wrong.
//   // }
//   clocks_init();

//   delay(1000);
// }


#endif
