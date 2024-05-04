#include <Arduino.h>
#include <AxisJoystick.h>
#include <LcdMenu.h>
#include <SimpleCLI.h>

const bool DEBUG = true;
const int BAUDRATE = 9600;
const unsigned int DELAY_TIME = 250;

const int X_PIN = A13, Y_PIN = A7, BUTTON_PIN = 26;
AxisJoystick joystick(BUTTON_PIN, X_PIN, Y_PIN);


void setup()
{
    Serial.begin(BAUDRATE);
    while (!Serial) { delay(10); }

    joystick.calibrate(0, 1023, 250);

    if (DEBUG)
    {
        Serial.println("Joystick test setup complete");
    }
}

void loop()
{
    delay(DELAY_TIME);
    switch (joystick.singleRead())
    {
        case Joystick::Move::PRESS:
            Serial.println("Button is pressed");
            break;
        case Joystick::Move::UP:
            Serial.println("Joystick is pressed up");
            break;
        case Joystick::Move::DOWN:
            Serial.println("Joystick is pressed down");
            break;
        case Joystick::Move::RIGHT:
            Serial.println("Joystick is pressed right");
            break;
        case Joystick::Move::LEFT:
            Serial.println("Joystick is pressed left");
            break;
        case Joystick::Move::NOT:
            // Serial.println("Joystick is not pressed");
            break;
    }
}
