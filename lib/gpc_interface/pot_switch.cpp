#include <pot_switch.h>


PotSwitch::PotSwitch(int potPin, int buttonPin)
{
    POT_PIN = potPin;
    BUTTON_PIN = buttonPin;
}

/*
 * Method to check the state of the button in the joystick.
 * This method should NOT be used as ISR.
 */
void PotSwitch::checkButton()
{
    // Check if we should poll the button or if the ISR is active
    if (INTERRUPT_ACTIVE) {

    }
    // Read the button state
    LAST_BUTTON_STATE = BUTTON_STATE;
    BUTTON_STATE = digitalRead(BUTTON_PIN);

    // If the button state has changed, set the flag
    if (LAST_BUTTON_STATE != BUTTON_STATE)
    {
        if (DEBUG)
        {
            Serial.print("Button state changed: ");
            Serial.println(BUTTON_STATE);
        }
    }
}

void PotSwitch::poll()
{
    // Check if the button is activated
    if (!INTERRUPT_ACTIVE)
    {
        checkButton();
    }

    // If the button is not pressed, we don't need to check the potentiometer
    if (!BUTTON_STATE)
    {
        return;
    } else {
        // Reset the button state
        BUTTON_STATE = false;
    }

    // Read the potentiometer value
    LAST_VALUE = CURRENT_VALUE;
    CURRENT_VALUE = analogRead(POT_PIN);

    // If the value has changed, set the flag
    if (LAST_VALUE != CURRENT_VALUE)
    {
        if (DEBUG)
        {
            Serial.print("Pot value changed: ");
            Serial.println(CURRENT_VALUE);
        }
    }
}

bool PotSwitch::isActivated()
{
    return BUTTON_STATE;
}

bool PotSwitch::valueChanged()
{
    return LAST_VALUE != analogRead(POT_PIN);
}

int PotSwitch::getValue()
{
    return CURRENT_VALUE;
}

bool PotSwitch::getButtonState()
{
    return BUTTON_STATE;
}

void PotSwitch::setDebug(bool debug)
{
    DEBUG = debug;
}

void PotSwitch::setInterruptActive(bool state)
{
    INTERRUPT_ACTIVE = state;
}

void PotSwitch::buttonTriggered()
{
    BUTTON_STATE = true;
}


