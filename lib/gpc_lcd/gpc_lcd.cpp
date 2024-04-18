#include "gpc_lcd.h"

// Global configuration
const int LCD_ADDRESS = 0x27;
const int LCD_WIDTH = 20;
const int LCD_HEIGHT = 4;

String initMessage = "Initializing...";
String welcomeMessage = "Welcome!";


// MY_LCD 
MY_LCD::MY_LCD(int address, int width, int height) : LiquidCrystal_I2C(address, width, height)
{
    begin(width, height);
}

void MY_LCD::printInitMessage()
{
    clear();
    setCursor(getCenteredPosition(initMessage), 0);
}

