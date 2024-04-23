#include <Arduino.h>

const int CASE_INTRUSION_PIN = 9;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        delay(10);
    }
    pinMode(CASE_INTRUSION_PIN, INPUT);
}

void loop()
{
    int caseIntrusion = digitalRead(CASE_INTRUSION_PIN);
    Serial.println(caseIntrusion);
    delay(500);
}

