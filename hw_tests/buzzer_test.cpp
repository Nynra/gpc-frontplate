#include <Arduino.h>

const int BUZZER_PIN = 3;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        delay(10);
    }
    pinMode(BUZZER_PIN, OUTPUT);
}

void loop()
{
    tone(BUZZER_PIN, 1000);
    delay(1000);
    noTone(BUZZER_PIN);
    delay(1000);
}