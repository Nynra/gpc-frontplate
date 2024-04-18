#include <Arduino.h>


/*
 * This class is meant to handle a potentionmeter with an activation button.
 */
class PotSwitch 
{
public:
    // Constructor and setup
    PotSwitch(int potPin, int buttonPin);

    // Polling methods
    void poll();
    bool isActivated();
    bool valueChanged();

    // Setters and getters
    int getValue();
    bool getButtonState();
    void setDebug(bool debug);
    void setInterruptActive(bool state);

    // Interrupt service routines
    void buttonTriggered();
private:
    int POT_PIN, BUTTON_PIN;
    int LAST_VALUE = 0, CURRENT_VALUE = 0;
    bool LAST_BUTTON_STATE = false, BUTTON_STATE = false, POT_VALUE_CHANGED = false;
    bool DEBUG = false, INTERRUPT_ACTIVE = false;

    void checkButton();
    void readPotValue();
};
