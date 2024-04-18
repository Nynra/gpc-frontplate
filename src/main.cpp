#include <Arduino.h>
#include <pot_switch.h>
#include <rot_switch.h>
#include <joystick.h>
// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>
// #include <SimpleCLI.h>
// #include <LcdMenu.h>
// #include <utils/commandProccesors.h>

/*
 * The following will be connected:
 * - 5x Switches on the frontplate
 * - 1x I2C LCD 20x4
 * - 4x Relays internally to switch power of components
 *     - EPS router
 *     - Teensy
 *     - Raspberry Pi 5
 *     - Opamp circuits
 * - 1x Buzzer for warnings
 * - 1x Flame sensor for fire detection in the case
 * - 1x Potentiometer with button
 * - 1x Joystick with button
 */

// const int LCD_ADDRESS = 0x3F;

// Global configuration
const int BAUD_RATE = 9600;
bool DEBUG = true;

// PIN CONFIG
// Joystick
const int JOY_BUTTON_PIN = 2;
const int JOY_X_PIN = A1, JOY_Y_PIN = A2;

// Pot and rotary switch
const int POT_SWITCH_PIN = 3;
const int POT_1_PIN = A3;
const int ROT_SWITCH_1_PIN = 4, ROT_SWITCH_2_PIN = 5, ROT_SWITCH_3_PIN = 6, ROT_SWITCH_4_PIN = 7, ROT_SWITCH_5_PIN = 8;

// Power button pins
const int POWER_BUTTON_1_PIN = 9, POWER_BUTTON_2_PIN = 10, POWER_BUTTON_3_PIN = 11, POWER_BUTTON_4_PIN = 12, POWER_BUTTON_5_PIN = 13;

// Power relays
const int RELAY_1_PIN = 14, RELAY_2_PIN = 15, RELAY_3_PIN = 16, RELAY_4_PIN = 17;

// Buzzer, case intrusion, flame sensor and psu monitor
const int BUZZER_PIN = 18, CASE_INTRUSION_PIN = 19;
const int FLAME_SENSOR_PIN = A4, PSU_MONITOR_PIN = A5;

/*
 * Create the frontplate instances
 */

RotSwitch r_switch(ROT_SWITCH_1_PIN, ROT_SWITCH_2_PIN, ROT_SWITCH_3_PIN, ROT_SWITCH_4_PIN, ROT_SWITCH_5_PIN);
PotSwitch p_switch(POT_1_PIN, POT_SWITCH_PIN);
Joystick joystick(JOY_X_PIN, JOY_Y_PIN, JOY_BUTTON_PIN);


/*
 * Define the menu structure
 */

