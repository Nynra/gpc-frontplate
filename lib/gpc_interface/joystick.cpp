#include <joystick.h>


/*
 * Constructor for the joystick.
 * The constructor is responsible for setting the pins
 */
Joystick::Joystick(int xPin, int yPin, int buttonPin)
{
    X_PIN = xPin;
    Y_PIN = yPin;
    BUTTON_PIN = buttonPin;
}

void Joystick::poll()
{
    checkButton();
    readValues();
}

/*
 * Method to check the state of the button in the joystick.
 * This method should NOT be used as ISR.
 */
void Joystick::checkButton()
{
    if (INTERRUPT_ACTIVE)
    {
        return;
    } else {
        // Read the button state
        BUTTON_STATE = digitalRead(BUTTON_PIN);
    }
}

/*
 * Method to read the values of the joystick.
 * This method should NOT be used as ISR as joystick movement is slow enough to be polled.
 */
void Joystick::readValues()
{
    // Read the values of the joystick
    X_VALUE = analogRead(X_PIN);
    Y_VALUE = analogRead(Y_PIN);
}

/*
 * Getter for the button state.
 * If an ISR for the button event is in use the state of the button will be returned to false
 * after the value has been read. This means that calling the method twice will return false.
 */
bool Joystick::getButtonState()
{
    if (BUTTON_STATE)
    {
        BUTTON_STATE = false;
        return true;
    }
    return false;
}

/*
 * ISR to be used for the rising edge of the button.
 * The method is only used to mark when the button is triggered.
 */
void Joystick::buttonTriggered()
{
    BUTTON_STATE = true;
}

/*
 * Getter for the X value of the joystick.
 */
int Joystick::getXValue()
{
    return X_VALUE;
}

/*
 * Getter for the Y value of the joystick.
 */
int Joystick::getYValue()
{
    return Y_VALUE;
}

/*
 * Setter for the debug flag.
 * If the debug flag is set to true, the joystick will print debug information to the serial monitor.
 */
void Joystick::setDebug(bool debug)
{
    DEBUG = debug;
}

/*
 * Setter for the interrupt active flag.
 * If the interrupt active flag is set to true, the joystick will use the ISR for the button.
 */
void Joystick::setInterruptActive(bool state)
{
    INTERRUPT_ACTIVE = state;
}


/*
 * Setter for the deadzone of the joystick.
 * The deadzone is the area around the center of the joystick where the joystick is considered to be in the center.
 */
void Joystick::setDeadzone(int deadzone)
{
    DEADZONE = deadzone;
}

/*
 * Getter for the deadzone of the joystick.
 */
int Joystick::getDeadzone()
{
    return DEADZONE;
}


