#include <Arduino.h>

const int FIRE_SENSOR_PIN = A7;
int FIRE_SENSOR_THRESHOLD = 800;
const int FIRE_SENSOR_TRIGGER_PIN = 8;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        delay(10);
    }
    pinMode(FIRE_SENSOR_TRIGGER_PIN, OUTPUT);
}

void loop()
{
    int flameSensorValue = analogRead(FIRE_SENSOR_PIN);
    bool flameTriggerValue = digitalRead(FIRE_SENSOR_TRIGGER_PIN);
    if (flameSensorValue < FIRE_SENSOR_THRESHOLD)
    {
        Serial.print("Fire Detected using analog pin: ");
        Serial.println(flameSensorValue);
    }
    if (flameTriggerValue == HIGH)
    {
        Serial.println("Fire Detected using digital pin");
    }
    delay(250);
}