#include <Arduino.h>


/*
 * This class is meant to handle a potentionmeter with an activation button.
 */
class PotSwitch 
{
public:
    // Constructor and setup
    PotSwitch(int potPin, int buttonPin);
    void setup();

    // Polling methods
    void poll();
    bool isActive();

    // Setters and getters
    int getPotValue();
    bool getButtonState();
    void setDebug(bool debug);

private:
    int POT_PIN, BUTTON_PIN;
    bool SETUP_DONE = false, BUTTON_STATE;
    int POT_VALUE;
    bool DEBUG = false;
};
