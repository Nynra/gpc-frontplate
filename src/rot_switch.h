#include <Arduino.h>


/*
 * This class handles a rotary switch with 5 positions.
 */
class RotSwitch
{
public:
    // Constructor and setup
    RotSwitch(int pin1, int pin2, int pin3, int pin4, int pin5);
    void setup();

    // Polling methods
    void poll();

    // Setters and getters
    void setDebug(bool debug);
    int getValue();
    int getChangeDirection();
private:
    int PIN_1, PIN_2, PIN_3, PIN_4, PIN_5;
    int CURRENT_POSITION = 0, LAST_POSITION = 0, MOVEMENT_DIRECTION = 0;
    bool DEBUG = false, SETUP_DONE = false;

    int findPosition();
    bool valueChanged();
};