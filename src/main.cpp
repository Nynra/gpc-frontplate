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

// Hardware dependancies
#include <Arduino.h>
#include <pot_switch.h>
#include <rot_switch.h>
#include <LiquidCrystal_I2C.h>

// Software dependancies
#include <MenuItem.h>
#include <ItemSubMenu.h>
#include <ItemCommand.h>
#include <ItemToggle.h>
#include <ItemProgress.h>
#include <ItemInput.h>
#include <LcdMenu.h>
#include <AxisJoystick.h>

const float VERSION = 0.001;

// Global configuration
const int BAUD_RATE = 9600;
const bool DEBUG = true;
const int LCD_ADDRESS = 0x3F;
const int LCD_ROWS = 4, LCD_COLS = 20;
const int ARDUINO_I2C_ADDRESS = 15;

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


/*
 * Switch states and values
 */
bool POWER_BUTTON_1_STATE, POWER_BUTTON_2_STATE, POWER_BUTTON_3_STATE, POWER_BUTTON_4_STATE, POWER_BUTTON_5_STATE;

// Function prototypes
void move_back();
void read_power_buttons();
void move_joystick();

char* intMapping(uint16_t progress);
char* floatMapping(uint16_t progress);
char* boolMapping(uint16_t progress);

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
         ITEM_COMMAND("Back", move_back));

SUB_MENU(modesMenu, mainMenu,
         ITEM_BASIC("DMM"),
         ITEM_BASIC("FunctionGenerator"),
         ITEM_BASIC("XY-Stage"),
         ITEM_BASIC("DataLogger"),
         ITEM_COMMAND("Back", move_back));

SUB_MENU(rootSettingsMenu, mainMenu,
        //  ITEM_SUBMENU("Safety", safetyMenu),
         ITEM_SUBMENU("Display", displayMenu),
         ITEM_COMMAND("Back", move_back));


SUB_MENU(displayMenu, rootSettingsMenu,
         ITEM_BASIC("Brightness"),
         ITEM_BASIC("Contrast"),
         ITEM_COMMAND("Back", move_back));


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

  // Initialize the power button pins
  pinMode(POWER_BUTTON_1_PIN, INPUT);
  pinMode(POWER_BUTTON_2_PIN, INPUT);
  pinMode(POWER_BUTTON_3_PIN, INPUT);
  pinMode(POWER_BUTTON_4_PIN, INPUT);
  pinMode(POWER_BUTTON_5_PIN, INPUT);

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
  // Read the power button states
  read_power_buttons();

  // Read the potentiometer and rotary switch
  p_switch.poll();
  r_switch.poll();

  // Read the joystick
  move_joystick();

}

void move_joystick()
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

void move_back()
{
  lcd_menu.back();
}

void read_power_buttons()
{
  POWER_BUTTON_1_STATE = digitalRead(POWER_BUTTON_1_PIN);
  POWER_BUTTON_2_STATE = digitalRead(POWER_BUTTON_2_PIN);
  POWER_BUTTON_3_STATE = digitalRead(POWER_BUTTON_3_PIN);
  POWER_BUTTON_4_STATE = digitalRead(POWER_BUTTON_4_PIN);
  POWER_BUTTON_5_STATE = digitalRead(POWER_BUTTON_5_PIN);
}

/*
 * Conversion methods for displaying numbers in the menu
 */

char* intMapping(uint16_t progress) {
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

char* floatMapping(uint16_t progress) {
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

char* boolMapping(uint16_t progress) {
    // Map the progress value to a boolean value
    bool boolValue = progress > 0;

    // Buffer to store the converted string
    static char buffer[10];

    // Convert the boolean value to a string
    itoa(boolValue, buffer, 10);

    // Return the resulting string
    return buffer;
}


void displayCallback(){
    // Display the menu
    // lcd_menu.display();
}