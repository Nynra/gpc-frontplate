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


const bool DEBUG = true;
const int BAUDRATE = 9600;
const unsigned int DELAY_TIME = 100;

const int LCD_ADDRESS = 0x27;
const int LCD_ROWS = 4, LCD_COLS = 20;

/*
 * Define the pin configuration
 */
const int INTERNAL_LED_PIN = 13;
// Joystick
const int JOY_BUTTON_PIN = 27;
const int JOY_X_PIN = A13;
const int JOY_Y_PIN = A7;

// Function prototypes
void menuMoveBackCallback();
void toggleBacklightCallback(uint16_t isOn);
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
// extern MenuItem *modesMenu[];
extern MenuItem *infoMenu[];

// Define the menus
MAIN_MENU(
    ITEM_SUBMENU("Info", infoMenu),
    ITEM_SUBMENU("Settings", rootSettingsMenu));

SUB_MENU(infoMenu, mainMenu,
         ITEM_BASIC("Version"),
         ITEM_COMMAND("Back", menuMoveBackCallback));

SUB_MENU(rootSettingsMenu, mainMenu,
            ITEM_TOGGLE("LCD Backlight", "ON", "OFF", toggleBacklightCallback),
            ITEM_COMMAND("Back", menuMoveBackCallback));

AxisJoystick joystick(JOY_BUTTON_PIN, JOY_X_PIN, JOY_Y_PIN);
LcdMenu lcd_menu(LCD_ROWS, LCD_COLS);

void menuMoveBackCallback() { lcd_menu.back(); }

void toggleBacklightCallback(uint16_t isOn) { lcd_menu.setBacklight(isOn); }

void setup()
{
    Serial.begin(BAUDRATE);
    while (!Serial)
    {
        delay(10);
    }

    pinMode(INTERNAL_LED_PIN, OUTPUT);

    joystick.calibrate(0, 1023, 250);

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

    // byte command = 0;
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
