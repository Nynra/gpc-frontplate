#include <pot_switch.h>


PotSwitch::PotSwitch(int potPin, int buttonPin)
{
    POT_PIN = potPin;
    BUTTON_PIN = buttonPin;
}

void PotSwitch::setup()
{
    if (!SETUP_DONE)
    {
        if (DEBUG)
        {
            Serial.println("INFO: Setting up potentiometer with button");
        }
    } else {
        if (DEBUG)
        {
            Serial.println("WARNING: Potentiometer with button already setup");
        }
    }

    // Set the pins as input
    pinMode(POT_PIN, INPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Read the initial values
    POT_VALUE = analogRead(POT_PIN);
    BUTTON_STATE = digitalRead(BUTTON_PIN);

    // Set the setup flag
    SETUP_DONE = true;
}

void PotSwitch::poll()
{
    // Check the button state
    BUTTON_STATE = digitalRead(BUTTON_PIN);

    // Read the pot value
    int value = analogRead(POT_PIN);
    POT_VALUE = map(value, 0, 1023, 0, 100);

    if (DEBUG)
    {
        Serial.print("INFO: Pot value: ");
        Serial.print(POT_VALUE);
        Serial.print(" Button state: ");
        Serial.println(BUTTON_STATE);
    }
}

bool PotSwitch::isActive()
{
    return BUTTON_STATE;
}

int PotSwitch::getPotValue()
{
    return POT_VALUE;
}

bool PotSwitch::getButtonState()
{
    return BUTTON_STATE;
}

void PotSwitch::setDebug(bool debug)
{
    DEBUG = debug;
}


