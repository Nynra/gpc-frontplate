#include <Arduino.h>
#include <pot_switch.h>

const bool DEBUG = true;
const int BAUDRATE = 9600;
const unsigned int DELAY_TIME = 100;

const bool TEST_PIN_OUTPUTS = true;
const bool TEST_POLLING = false;

const int POT_PIN = A3, BUTTON_PIN = 7;
PotSwitch p_switch(POT_PIN, BUTTON_PIN);

void setup()
{
    Serial.begin(BAUDRATE);
    while (!Serial) { delay(10); }
    p_switch.setup();

    if (DEBUG)
    {
        Serial.println("Potentiometer with switch test setup complete");
    }
}

void loop()
{
    p_switch.poll();
    if (TEST_PIN_OUTPUTS)
    {
        Serial.print("Pot value: ");
        Serial.print(analogRead(POT_PIN));
        Serial.print(" Button state: ");
        Serial.println(digitalRead(BUTTON_PIN));
    }
    if (TEST_POLLING)
    {
        Serial.print("Pot value: ");
        Serial.print(p_switch.getPotValue());
        Serial.print(" Button state: ");
        Serial.println(p_switch.getButtonState());
    }

    delay(DELAY_TIME);
}