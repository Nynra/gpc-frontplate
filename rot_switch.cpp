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
    CURRENT_POSITION = findPosition();
    LAST_POSITION = CURRENT_POSITION;
    SETUP_DONE = true;
}

bool RotSwitch::valueChanged()
{
    int pos = findPosition();
    if (pos != CURRENT_POSITION)
    {
        if (DEBUG)
        {
            Serial.println("The position changed");
        }
        LAST_POSITION = CURRENT_POSITION;
        CURRENT_POSITION = pos;
        if (CURRENT_POSITION > LAST_POSITION)
        {
            MOVEMENT_DIRECTION = 1;
        } else if (CURRENT_POSITION < LAST_POSITION)
        {
            MOVEMENT_DIRECTION = -1;
        }
        Serial.println(MOVEMENT_DIRECTION);
        LAST_POSITION = CURRENT_POSITION;
        return true;
    }
    MOVEMENT_DIRECTION = 0;
    return false;

}

int RotSwitch::findPosition()
{
    bool value1 = digitalRead(PIN_1);
    bool value2 = digitalRead(PIN_2);
    bool value3 = digitalRead(PIN_3);
    bool value4 = digitalRead(PIN_4);
    bool value5 = digitalRead(PIN_5);
    int return_pin = 0;

    // Only one should be high
    if (value1 + value2 + value3 + value4 + value5 != 1)
    {
        if (DEBUG)
        {
            Serial.print("ERROR: More than one rotary switch pin is high: ");
            Serial.println(value1 + value2 + value3 + value4 + value5);
        }
        return 0;
    } else if (value1 + value2 + value3 + value4 + value5 == 0)
    {
        if (DEBUG)
        {
            Serial.println("ERROR: No rotary switch pin is high");
        }
        return 0;
    } else {
        if (value1)
        {
            return_pin = 1;
        } else if (value2)
        {
            return_pin = 2;
        } else if (value3)
        {
            return_pin = 3;
        } else if (value4)
        {
            return_pin = 4;
        } else if (value5)
        {
            return_pin = 5;
        }
    }
    return return_pin;
}

void RotSwitch::poll()
{
    if (valueChanged())
    {
        if (DEBUG)
        {
            Serial.print("Rotary switch position changed to: ");
            Serial.println(CURRENT_POSITION);
        }
    } else {
        if (DEBUG)
        {
            Serial.print(".");
        }
    
    }
}

/*
 * Setters and getters
 */

// Set the debug state 
void RotSwitch::setDebug(bool debug)
{
    DEBUG = debug;
}

// Get the number of the currently active switch
int RotSwitch::getValue()
{
    return CURRENT_POSITION;
}

// Get the rate of change between now and the last cycle
int RotSwitch::getChangeDirection()
{
    return MOVEMENT_DIRECTION;
}