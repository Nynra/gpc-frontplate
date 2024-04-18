#include <Arduino.h>


/*
 * This class is meant to handle two potentionmeters with a button.
 */
class Joystick
{
public:
    // Constructor and setup
    Joystick(int xPin, int yPin, int buttonPin);

    // Polling methods
    void poll();

    // Setters and getters
    bool getButtonState();
    int getXValue();
    int getYValue();
    void setDeadzone(int deadzone);
    int getDeadzone();
    void setDebug(bool debug);
    void setInterruptActive(bool state);

    // Interrupt service routines
    void buttonTriggered();

private:
    // Variable declarations
    int X_PIN, Y_PIN, BUTTON_PIN;
    int X_VALUE = 0, Y_VALUE = 0;
    int DEADZONE = 30;
    volatile bool BUTTON_STATE = false;
    bool DEBUG = false, INTERRUPT_ACTIVE = false;

    // Method declarations
    void checkButton();
    void readValues();
};
