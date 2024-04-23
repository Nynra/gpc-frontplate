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

#include <Arduino.h>

// Menu dependancies
#include <MenuItem.h>
#include <ItemSubMenu.h>
#include <ItemCommand.h>
#include <ItemToggle.h>
#include <ItemProgress.h>
#include <ItemInput.h>
#include <LcdMenu.h>

// Frontplate dependancies
#include <AxisJoystick.h>
#include <pot_switch.h>
#include <rot_switch.h>

const float VERSION = 0.001;

// Global configuration
const int BAUD_RATE = 9600;
const bool DEBUG = true;
const int LCD_ADDRESS = 0x3F;
const int LCD_ROWS = 4, LCD_COLS = 20;

/*
 * Define the pin configuration
 */
// Joystick
const int JOY_BUTTON_PIN = 2;
const int JOY_X_PIN = A1, JOY_Y_PIN = A2;

// Pot and rotary switch
const int POT_SWITCH_PIN = 3;
const int POT_1_PIN = A3;
const int ROT_SWITCH_1_PIN = 4, ROT_SWITCH_2_PIN = 5, ROT_SWITCH_3_PIN = 6, ROT_SWITCH_4_PIN = 7, ROT_SWITCH_5_PIN = 8;

// Power button pins
const int POWER_BUTTON_1_PIN = 9, POWER_BUTTON_2_PIN = 10, POWER_BUTTON_3_PIN = 11, POWER_BUTTON_4_PIN = 12, POWER_BUTTON_5_PIN = 13;

const int RPI_RELAY_PIN = 7;
const int TEENSY_RELAY_PIN = 6;
const int ESP32_RELAY_PIN = 5;
const int OPAMP_RELAY_PIN = 4;

const int CASE_INTRUSION_PIN = 9;
const int BUZZER_PIN = 3;
const int BUILDIN_LED_PIN = 13;

const int FIRE_SENSOR_PIN = A7;
const int RPI_POWER_MON_PIN = A1;
const int TEENSY_POWER_MON_PIN = A2;
const int ESP32_POWER_MON_PIN = A3;
const int OPAMP_POWER_MON_PIN = A0;

/*
 * State and threshold definitions
 */
// Power buttons
bool POWER_BUTTON_1_STATE = false;
bool POWER_BUTTON_2_STATE = false;
bool POWER_BUTTON_3_STATE = false;
bool POWER_BUTTON_4_STATE = false;
bool POWER_BUTTON_5_STATE = false;

// Relays
bool RPI_RELAY_STATE = false;
bool TEENSY_RELAY_STATE = false;
bool ESP32_RELAY_STATE = false;
bool OPAMP_RELAY_STATE = false;

// Sensors
int FIRE_SENSOR_THRESHOLD = 850;

// Power Monitors
int GENERAL_POWER_MON_THRESHOLD = 90; // % of the power

float RPI_POWER_MON_DIV_FACTOR = 1; // 1 if no divider is used
float RPI_POWER_MON_VALUE = 0;
int RPI_POWER_MON_THRESHOLD = GENERAL_POWER_MON_THRESHOLD;

float TEENSY_POWER_MON_DIV_FACTOR = 1; // 3.3V / 5V approx 0.6
float TEENSY_POWER_MON_VALUE = 0;
int TEENSY_POWER_MON_THRESHOLD = GENERAL_POWER_MON_THRESHOLD;

float ESP32_POWER_MON_DIV_FACTOR = 1;
float ESP32_POWER_MON_VALUE = 0;
int ESP32_POWER_MON_THRESHOLD = GENERAL_POWER_MON_THRESHOLD;

float OPAMP_POWER_MON_DIV_FACTOR = 1;
float OPAMP_POWER_MON_VALUE = 0;
int OPAMP_POWER_MON_THRESHOLD = GENERAL_POWER_MON_THRESHOLD;

// Error states (Flashing LED)
bool RPI_POWER_MON_ERROR_STATE = false;
bool TEENSY_POWER_MON_ERROR_STATE = false;
bool ESP32_POWER_MON_ERROR_STATE = false;
bool OPAMP_POWER_MON_ERROR_STATE = false;

// Emergency states (Flashing LED, power cut and buzzer on)
bool CASE_INTRUSION_ERROR_STATE = false;
bool FIRE_SENSOR_ERROR_STATE = false;

const int NUM_ERROR_FLAGS = 6;

// Function prototypes
void menuMoveBack();
char *intMapping(uint16_t progress);
char *floatMapping(uint16_t progress);
char *boolMapping(uint16_t progress);

void updateButtons();
void updateJoystickMove();
void updateRelays();
void updatePowerMonitors();
void updateErrorStates();

/*
 * Define the menu structure
 * MainMenu
 * |
 * |- Info
 * |
 * |- Modes
 * |  |- DMM
 * |  |- FunctionGenerator
 * |  |- XY-Stage
 * |  |- DataLogger
 * |
 * |- Settings
 * |  |- Safety
 * |  |- Display
 */

// Define the menus objects
extern MenuItem *rootSettingsMenu[];
extern MenuItem *modesMenu[];
extern MenuItem *infoMenu[];
extern MenuItem *displayMenu[];
extern MenuItem *safetyMenu[];

// Define the callbacks

// Define the menus
MAIN_MENU(
    ITEM_SUBMENU("Info", infoMenu),
    ITEM_SUBMENU("Modes", modesMenu),
    ITEM_SUBMENU("Settings", rootSettingsMenu));

SUB_MENU(infoMenu, mainMenu,
         ITEM_PROGRESS("Version", VERSION, intMapping, NULL),
         ITEM_COMMAND("Back", menuMoveBack));

SUB_MENU(modesMenu, mainMenu,
         ITEM_BASIC("DMM"),
         ITEM_BASIC("FunctionGenerator"),
         ITEM_BASIC("XY-Stage"),
         ITEM_BASIC("DataLogger"),
         ITEM_COMMAND("Back", menuMoveBack));

SUB_MENU(rootSettingsMenu, mainMenu,
         //  ITEM_SUBMENU("Safety", safetyMenu),
         ITEM_SUBMENU("Display", displayMenu),
         ITEM_COMMAND("Back", menuMoveBack));

SUB_MENU(displayMenu, rootSettingsMenu,
         ITEM_BASIC("Brightness"),
         ITEM_BASIC("Contrast"),
         ITEM_COMMAND("Back", menuMoveBack));

/*
 * Create the frontplate instances
 */
RotSwitch r_switch(ROT_SWITCH_1_PIN, ROT_SWITCH_2_PIN, ROT_SWITCH_3_PIN, ROT_SWITCH_4_PIN, ROT_SWITCH_5_PIN);
PotSwitch p_switch(POT_1_PIN, POT_SWITCH_PIN);
AxisJoystick joystick(JOY_X_PIN, JOY_Y_PIN, JOY_BUTTON_PIN);
LcdMenu lcd_menu(LCD_ROWS, LCD_COLS);

void setup()
{
  // Initialize the serial connection
  Serial.begin(BAUD_RATE);
  while (!Serial)
  {
    delay(10);
  }

  if (DEBUG)
  {
    Serial.println("INFO: Serial connection established");
  }

  // Initialize the input pins
  pinMode(POWER_BUTTON_1_PIN, INPUT);
  pinMode(POWER_BUTTON_2_PIN, INPUT);
  pinMode(POWER_BUTTON_3_PIN, INPUT);
  pinMode(POWER_BUTTON_4_PIN, INPUT);
  pinMode(POWER_BUTTON_5_PIN, INPUT);
  pinMode(CASE_INTRUSION_PIN, INPUT);
  pinMode(RPI_RELAY_PIN, OUTPUT);
  pinMode(TEENSY_RELAY_PIN, OUTPUT);
  pinMode(ESP32_RELAY_PIN, OUTPUT);
  pinMode(OPAMP_RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUILDIN_LED_PIN, OUTPUT);

  // Initialize the frontplate components
  r_switch.setup();
  p_switch.setup();
  // joystick.setup();

  // Initialize the LCD
  lcd_menu.setupLcdWithMenu(LCD_ADDRESS, mainMenu);

  // Start the I2C communication as Controller
  // Wire.begin();

  if (DEBUG)
  {
    Serial.println("INFO: Setup complete");
  }
}

void loop()
{
  // Run the update functions
  updateButtons();
  updatePowerMonitors();
  updateRelays();
  updateErrorStates();
  delay(50);  // wait for a bit
}

void menuMoveBack()
{
  lcd_menu.back();
}

/*
 * Conversion methods for displaying numbers in the menu
 */

char *intMapping(uint16_t progress)
{
  // Map the progress value to a new range (100 to 200)
  long mapped = mapProgress(progress, 100L, 200L);

  // Buffer to store the converted stringV
  static char buffer[10];

  // Convert the mapped value to a string
  itoa(mapped, buffer, 10);

  // Concatenate the string with the character 'm'
  concat(buffer, 'm', buffer);

  // Return the resulting string
  return buffer;
}

char *floatMapping(uint16_t progress)
{
  // Normalize the progress value and map it to the specified floating-point
  // range
  float floatValue = mapProgress(progress, -1.0f, 1.0f);

  // Buffer to store the converted string
  // static char buffer[10]; // On Arduino
  char buffer[20];

  // Convert the floating-point value to a string with 4 characters (including
  // decimal point) and 2 decimal places
  // dtostrf(floatValue, 4, 2, buffer);  // On Arduino
  snprintf(buffer, sizeof(buffer), "%4.2f", floatValue);

  // Concatenate the string with the character 'A'
  concat(buffer, 'A', buffer);

  // Return the resulting string
  return buffer;
}

char *boolMapping(uint16_t progress)
{
  // Map the progress value to a boolean value
  bool boolValue = progress > 0;

  // Buffer to store the converted string
  static char buffer[10];

  // Convert the boolean value to a string
  itoa(boolValue, buffer, 10);

  // Return the resulting string
  return buffer;
}

/*
 * Updating methods
 */

// Update the frondplate buttons
void updateButtons()
{
  POWER_BUTTON_1_STATE = digitalRead(POWER_BUTTON_1_PIN);
  POWER_BUTTON_2_STATE = digitalRead(POWER_BUTTON_2_PIN);
  POWER_BUTTON_3_STATE = digitalRead(POWER_BUTTON_3_PIN);
  POWER_BUTTON_4_STATE = digitalRead(POWER_BUTTON_4_PIN);
  POWER_BUTTON_5_STATE = digitalRead(POWER_BUTTON_5_PIN);

  // Read the potentiometer and rotary switch
  p_switch.poll();
  r_switch.poll();
  updateJoystickMove();
}

void updateJoystickMove()
{
  // Poll the joystic movement
  if (joystick.isUp())
  {
    lcd_menu.up();
  }
  else if (joystick.isDown())
  {
    lcd_menu.down();
  }
  else if (joystick.isLeft())
  {
    lcd_menu.left();
  }
  else if (joystick.isRight())
  {
    lcd_menu.right();
  }
  else if (joystick.isPress())
  {
    lcd_menu.enter();
  }
}

// Update the relay states
void updateRelays()
{
  digitalWrite(RPI_RELAY_PIN, RPI_RELAY_STATE);
  digitalWrite(TEENSY_RELAY_PIN, TEENSY_RELAY_STATE);
  digitalWrite(ESP32_RELAY_PIN, ESP32_RELAY_STATE);
  digitalWrite(OPAMP_RELAY_PIN, OPAMP_RELAY_STATE);
}

// Update the power monitor values
void updatePowerMonitors()
{
  RPI_POWER_MON_VALUE = analogRead(RPI_POWER_MON_PIN) / RPI_POWER_MON_DIV_FACTOR;
  TEENSY_POWER_MON_VALUE = analogRead(TEENSY_POWER_MON_PIN) / TEENSY_POWER_MON_DIV_FACTOR;
  ESP32_POWER_MON_VALUE = analogRead(ESP32_POWER_MON_PIN) / ESP32_POWER_MON_DIV_FACTOR;
  OPAMP_POWER_MON_VALUE = analogRead(OPAMP_POWER_MON_PIN) / OPAMP_POWER_MON_DIV_FACTOR;
}

// Update the error states
void updateErrorStates()
{
  // Update the RPI power monitor error state
  if (RPI_POWER_MON_VALUE < RPI_POWER_MON_THRESHOLD && RPI_RELAY_STATE)
  {
    RPI_POWER_MON_ERROR_STATE = true;
  }
  else
  {
    RPI_POWER_MON_ERROR_STATE = false;
  }

  // Update the Teensy power monitor error state
  if (TEENSY_POWER_MON_VALUE < TEENSY_POWER_MON_THRESHOLD && TEENSY_RELAY_STATE)
  {
    TEENSY_POWER_MON_ERROR_STATE = true;
  }
  else
  {
    TEENSY_POWER_MON_ERROR_STATE = false;
  }

  // Update the ESP32 power monitor error state
  if (ESP32_POWER_MON_VALUE < ESP32_POWER_MON_THRESHOLD && ESP32_RELAY_STATE)
  {
    ESP32_POWER_MON_ERROR_STATE = true;
  }
  else
  {
    ESP32_POWER_MON_ERROR_STATE = false;
  }

  // Update the OPAMP power monitor error state
  if (OPAMP_POWER_MON_VALUE < OPAMP_POWER_MON_THRESHOLD && OPAMP_RELAY_STATE)
  {
    OPAMP_POWER_MON_ERROR_STATE = true;
  }
  else
  {
    OPAMP_POWER_MON_ERROR_STATE = false;
  }

  // Update the case intrusion error state
  if (digitalRead(CASE_INTRUSION_PIN) == HIGH)
  {
    CASE_INTRUSION_ERROR_STATE = true;
  }
  else
  {
    CASE_INTRUSION_ERROR_STATE = false;
  }

  // Update the fire sensor error state
  if (analogRead(FIRE_SENSOR_PIN) > FIRE_SENSOR_THRESHOLD)
  {
    FIRE_SENSOR_ERROR_STATE = true;
  }
  else
  {
    FIRE_SENSOR_ERROR_STATE = false;
  }
}
