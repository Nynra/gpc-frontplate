#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
#include <menu.h>




// LCD
class MY_LCD : public LiquidCrystal_I2C
{
public:
    // Constructor
    MY_LCD(int, int, int);

    // Methods
    void printInitMessage();
    void printWelcomeMessage();
    void clear() { this->clear(); }
    void displayMenu

private:
    int getCenteredPosition(String);
};