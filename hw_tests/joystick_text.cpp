#include <Arduino.h>
#include <joystick.h>

const bool DEBUG = true;
const int BAUDRATE = 9600;
const unsigned int DELAY_TIME = 100;

const int X_PIN = A0, Y_PIN = A1, BUTTON_PIN = 2;
Joystick joystick(X_PIN, Y_PIN, BUTTON_PIN);


void setup()
{
    Serial.begin(BAUDRATE);
    while (!Serial) { delay(10); }
    joystick.setup();

    if (DEBUG)
    {
        Serial.println("Joystick test setup complete");
    }
}

void loop()
{
    joystick.poll();
    delay(DELAY_TIME);
}
