#include <rot_switch.h>


/*
 * Constructor for the rotary switch.
 * The constructor is responsible for setting the pins
 */
RotSwitch::RotSwitch(int pin1, int pin2, int pin3, int pin4, int pin5)
{
    PIN_1 = pin1;
    PIN_2 = pin2;
    PIN_3 = pin3;
    PIN_4 = pin4;
    PIN_5 = pin5;
}

void RotSwitch::setup()
{
    if (!SETUP_DONE)
    {
        if (DEBUG)
        {
            Serial.println("INFO: Setting up rotary switch");
        }
    } else {
        if (DEBUG)
        {
            Serial.println("WARNING: Rotary switch already setup");
        }
    }

    // Set the pins as input
    pinMode(PIN_1, INPUT_PULLUP);
    pinMode(PIN_2, INPUT_PULLUP);
    pinMode(PIN_3, INPUT_PULLUP);
    pinMode(PIN_4, INPUT_PULLUP);
    pinMode(PIN_5, INPUT_PULLUP);

    // Check wich of the pins is high, only one should be high
    if (!digitalRead(PIN_1))
    {
        CURRENT_POSITION = 1;
    } else if (!digitalRead(PIN_2))
    {
        CURRENT_POSITION = 2;
    } else if (!digitalRead(PIN_3))
    {
        CURRENT_POSITION = 3;
    } else if (!digitalRead(PIN_4))
    {
        CURRENT_POSITION = 4;
    } else if (!digitalRead(PIN_5))
    {
        CURRENT_POSITION = 5;
    } else {
        if (DEBUG)
        {
            Serial.println("ERROR: No rotary switch pin is high");
        }
    }

    // Set the last position to the current position
    LAST_POSITION = CURRENT_POSITION;
    SETUP_DONE = true;
}

/*
 * Check if the position of the rotary switch has changed
 * This method can be used as ISR in combination with a hardware timer.
 */
bool RotSwitch::valueChanged()
{
    // Check if the last known switch pin is still the same
    if (!digitalRead(CURRENT_POSITION))
    {
        return true;
    }
    // The pin is still high so the position hasn't changed
    return false;

}

void RotSwitch::findPosition()
{
    // // Check which pin is high, starting from the last known pin +/- 1
    // for (int i = LAST_POSITION - 1; i <= LAST_POSITION + 1; i++)
    // {
    //     if (i == 0)
    //     {
    //         i = 5;
    //     } else if (i == 6)
    //     {
    //         i = 1;
    //     }

    //     if (!digitalRead(i))
    //     {
    //         CURRENT_POSITION = i;
    //         break;
    //     }
    // }

    if (!valueChanged())
    {
        return;
    } else {
        LAST_POSITION = CURRENT_POSITION;
    }

    // Check which of the pins is high
    if (!digitalRead(PIN_1))
    {
        CURRENT_POSITION = 1;
    } else if (!digitalRead(PIN_2))
    {
        CURRENT_POSITION = 2;
    } else if (!digitalRead(PIN_3))
    {
        CURRENT_POSITION = 3;
    } else if (!digitalRead(PIN_4))
    {
        CURRENT_POSITION = 4;
    } else if (!digitalRead(PIN_5))
    {
        CURRENT_POSITION = 5;
    } else {
        if (DEBUG)
        {
            Serial.println("ERROR: No rotary switch pin is high");
        }
    }

}

void RotSwitch::poll()
{
    // Check if the pin changed
    if (valueChanged())
    {
        if (DEBUG)
        {
            Serial.print("Rotary switch position changed: ");
            Serial.println(CURRENT_POSITION);
        }
        findPosition();
    }
}