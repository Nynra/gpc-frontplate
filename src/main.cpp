#include <Arduino.h>
#include <AxisJoystick.h>
#include <SimpleCLI.h>

// #include <MenuItem.h>
// #include <ItemProgress.h>
// #include <ItemInput.h>
#include <utils/commandProccesors.h>
#include <ItemSubMenu.h>
#include <ItemCommand.h>
#include <ItemToggle.h>
#include <LcdMenu.h> // Should always be imported as the last menu component

// Some global constants
const bool DEBUG = true;
const int BAUDRATE = 9600;
const unsigned int DELAY_TIME = 100;

const int LCD_ADDRESS = 0x27;
const int LCD_ROWS = 4, LCD_COLS = 20;

bool INTRUSION_PROTECTION_ACTIVE = true;
bool FIRE_ALARM_ACTIVE = true;

/*
 * Define the pin configuration and variables
 */
const int INTERNAL_LED_PIN = 13;
const int INTRUSION_PIN = 39;

bool INTRUSION_STATE = false;

// Joystick
const int JOY_BUTTON_PIN = 26;
const int JOY_X_PIN = A13;
const int JOY_Y_PIN = A7;

// Relays
const int RELAY_1_PIN = 39;
// const int RELAY_2_PIN = 40;
// const int RELAY_3_PIN = 41;
// const int RELAY_4_PIN = 42;

bool RELAY_1_STATE = false;
// bool RELAY_2_STATE = false;
// bool RELAY_3_STATE = false;
// bool RELAY_4_STATE = false;

// Buttons
const int BUTTON_1_PIN = 28;
const int BUTTON_2_PIN = 29;
const int BUTTON_3_PIN = 30;
const int BUTTON_4_PIN = 31;
const int BUTTON_5_PIN = 32;

bool BUTTON_1_STATE = false;
bool BUTTON_2_STATE = false;
bool BUTTON_3_STATE = false;
bool BUTTON_4_STATE = false;
bool BUTTON_5_STATE = false;

// Button LED's
const int BUTTON_1_LED_PIN = 11;
const int BUTTON_2_LED_PIN = 10;
const int BUTTON_3_LED_PIN = 9;
const int BUTTON_4_LED_PIN = 8;
const int BUTTON_5_LED_PIN = 7;


// Function prototypes
void menuMoveBack();
void toggleBacklight(uint16_t isOn);
void toggleIntrusionProtection(uint16_t isOn);
void toggleFireAlarm(uint16_t isOn);
void toggleRelay1(uint16_t isOn);
void toggleRelay2(uint16_t isOn);
void toggleRelay3(uint16_t isOn);
void toggleRelay4(uint16_t isOn);
// char *intMapping(uint16_t progress);
// char *floatMapping(uint16_t progress);
// char *boolMapping(uint16_t progress);

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
 * |  |   |- Configuration
 * |  |   |- Calibration
 * |  |   |- Control
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
extern MenuItem *relayPowerMenu[];
extern MenuItem *dmmMenu[];
extern MenuItem *functionGeneratorMenu[];
extern MenuItem *xyStageMenu[];
extern MenuItem *dataLoggerMenu[];

// Define the menus
MAIN_MENU(
    ITEM_SUBMENU("Info", infoMenu),
    ITEM_SUBMENU("Modes", modesMenu),
    ITEM_SUBMENU("Power", relayPowerMenu),
    ITEM_SUBMENU("Settings", rootSettingsMenu));

SUB_MENU(infoMenu, mainMenu,
         ITEM_BASIC("Version"),
         ITEM_COMMAND("Back", menuMoveBack));

SUB_MENU(rootSettingsMenu, mainMenu,
         // Init the backlight to say on
         ITEM_TOGGLE("Backlight ", "ON", "OFF", toggleBacklight),
         ITEM_TOGGLE("Anti intr.", "ON", "OFF", toggleIntrusionProtection),
         ITEM_TOGGLE("Fire alrm.", "ON", "OFF", toggleFireAlarm),
         ITEM_COMMAND("Back", menuMoveBack));

SUB_MENU(relayPowerMenu, mainMenu,
         ITEM_TOGGLE("RPI power   ", "ON", "OFF", toggleRelay1),
        //  ITEM_TOGGLE("ESP power   ", "ON", "OFF", toggleRelay2),
        //  ITEM_TOGGLE("Teensy power", "ON", "OFF", toggleRelay3),
        //  ITEM_TOGGLE("Opamp power ", "ON", "OFF", toggleRelay4),
         ITEM_COMMAND("Back", menuMoveBack));

SUB_MENU(modesMenu, mainMenu,
         ITEM_SUBMENU("DMM", dmmMenu),
         ITEM_SUBMENU("Function Generator", functionGeneratorMenu),
         ITEM_SUBMENU("XY-Stage", xyStageMenu),
         ITEM_SUBMENU("Data Logger", dataLoggerMenu),
         ITEM_COMMAND("Back", menuMoveBack));

SUB_MENU(dmmMenu, modesMenu,
         ITEM_BASIC("Configuration"),
         ITEM_COMMAND("Back", menuMoveBack));

SUB_MENU(functionGeneratorMenu, modesMenu,
         ITEM_BASIC("Configuration"),
         ITEM_COMMAND("Back", menuMoveBack));

SUB_MENU(xyStageMenu, modesMenu,
         ITEM_BASIC("Configuration"),
         ITEM_COMMAND("Back", menuMoveBack));

SUB_MENU(dataLoggerMenu, modesMenu,
         ITEM_BASIC("Configuration"),
         ITEM_COMMAND("Back", menuMoveBack));

AxisJoystick joystick(JOY_BUTTON_PIN, JOY_X_PIN, JOY_Y_PIN);
LcdMenu lcd_menu(LCD_ROWS, LCD_COLS);

void setup()
{
    Serial.begin(BAUDRATE);
    while (!Serial)
    {
        delay(10);
    }

    // Set the LED pins
    pinMode(INTERNAL_LED_PIN, OUTPUT);
    // pinMode(BUTTON_1_LED_PIN, OUTPUT);
    // pinMode(BUTTON_2_LED_PIN, OUTPUT);
    // pinMode(BUTTON_3_LED_PIN, OUTPUT);
    // pinMode(BUTTON_4_LED_PIN, OUTPUT);
    // pinMode(BUTTON_5_LED_PIN, OUTPUT);

    // Set the button pins
    pinMode(BUTTON_1_PIN, INPUT_PULLUP);
    pinMode(BUTTON_2_PIN, INPUT_PULLUP);
    pinMode(BUTTON_3_PIN, INPUT_PULLUP);
    pinMode(BUTTON_4_PIN, INPUT_PULLUP);
    pinMode(BUTTON_5_PIN, INPUT_PULLUP);

    // Set the relay pins
    pinMode(RELAY_1_PIN, OUTPUT);
    // pinMode(RELAY_2_PIN, OUTPUT);
    // pinMode(RELAY_3_PIN, OUTPUT);
    // pinMode(RELAY_4_PIN, OUTPUT);

    joystick.calibrate(0, 1023, 250);

    // Set some menu toggles to the right state
    rootSettingsMenu[1]->setIsOn(true);
    rootSettingsMenu[2]->setIsOn(INTRUSION_PROTECTION_ACTIVE);
    rootSettingsMenu[3]->setIsOn(FIRE_ALARM_ACTIVE);

    // Set the backlight toggle in the root settings to on
    // Initialize the LCD
    lcd_menu.setupLcdWithMenu(LCD_ADDRESS, mainMenu);

    if (DEBUG)
    {
        Serial.println("Joystick test setup complete");
    }
}

void loop()
{
    delay(DELAY_TIME);

    // Read the joystick to navigate the menu
    switch (joystick.singleRead())
    {
    case Joystick::Move::PRESS:
        lcd_menu.enter();
        break;
    case Joystick::Move::UP:
        lcd_menu.down();
        break;
    case Joystick::Move::DOWN:
        lcd_menu.up();
        break;
    case Joystick::Move::RIGHT:
        lcd_menu.left();
        break;
    case Joystick::Move::LEFT:
        lcd_menu.right();
        break;
    case Joystick::Move::NOT:
        // Serial.println("Joystick is not pressed");
        break;
    }
}

/*
 * Menu callback functions
 */
void menuMoveBack() { lcd_menu.back(); }

void toggleBacklight(uint16_t isOn) { lcd_menu.setBacklight(isOn); }

void toggleIntrusionProtection(uint16_t isOn) { INTRUSION_PROTECTION_ACTIVE = isOn; }

void toggleFireAlarm(uint16_t isOn) { FIRE_ALARM_ACTIVE = isOn; }

void toggleRelay1(uint16_t isOn)
{
    RELAY_1_STATE = isOn;
    digitalWrite(RELAY_1_PIN, isOn);
}

// void toggleRelay2(uint16_t isOn)
// {
//     RELAY_2_STATE = isOn;
//     digitalWrite(RELAY_2_PIN, isOn);
// }

// void toggleRelay3(uint16_t isOn)
// {
//     RELAY_3_STATE = isOn;
//     digitalWrite(RELAY_3_PIN, isOn);
// }

// void toggleRelay4(uint16_t isOn)
// {
//     RELAY_4_STATE = isOn;
//     digitalWrite(RELAY_4_PIN, isOn);
// }

/*
 * Updating functions
 */
void updateButtonsStates()
{
    // Buttons on the frond panel
    BUTTON_1_STATE = digitalRead(BUTTON_1_PIN);
    BUTTON_2_STATE = digitalRead(BUTTON_2_PIN);
    BUTTON_3_STATE = digitalRead(BUTTON_3_PIN);
    BUTTON_4_STATE = digitalRead(BUTTON_4_PIN);
    BUTTON_5_STATE = digitalRead(BUTTON_5_PIN);

    // Intrusion detection
    INTRUSION_STATE = digitalRead(INTRUSION_PIN);

    // Update the button LED's
    digitalWrite(BUTTON_1_LED_PIN, BUTTON_1_STATE);
    digitalWrite(BUTTON_2_LED_PIN, BUTTON_2_STATE);
    digitalWrite(BUTTON_3_LED_PIN, BUTTON_3_STATE);
    digitalWrite(BUTTON_4_LED_PIN, BUTTON_4_STATE);
    digitalWrite(BUTTON_5_LED_PIN, BUTTON_5_STATE);
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
    double floatValue = mapProgress(progress, -1.0f, 1.0f);

    // Buffer to store the converted string
    // static char buffer[10]; // On Arduino
    static char buffer[20];

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
