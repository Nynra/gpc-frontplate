#include <Arduino.h>

const int step_delay = 250;

// Button LED's
const int BUTTON_1_LED_PIN = 11;
const int BUTTON_2_LED_PIN = 10;
const int BUTTON_3_LED_PIN = 9;
const int BUTTON_4_LED_PIN = 8;
const int BUTTON_5_LED_PIN = 7;


void setup() {
    Serial.begin(9600);
    
    // put your setup code here, to run once:
    pinMode(BUTTON_1_LED_PIN, OUTPUT);
    pinMode(BUTTON_2_LED_PIN, OUTPUT);
    pinMode(BUTTON_3_LED_PIN, OUTPUT);
    pinMode(BUTTON_4_LED_PIN, OUTPUT);
    pinMode(BUTTON_5_LED_PIN, OUTPUT);
    
    Serial.println("Setup complete");
}

void loop() {
    // put your main code here, to run repeatedly:
    digitalWrite(BUTTON_1_LED_PIN, HIGH);
    delay(step_delay);
    digitalWrite(BUTTON_2_LED_PIN, HIGH);
    delay(step_delay);
    digitalWrite(BUTTON_3_LED_PIN, HIGH);
    delay(step_delay);
    digitalWrite(BUTTON_4_LED_PIN, HIGH);
    delay(step_delay);
    digitalWrite(BUTTON_5_LED_PIN, HIGH);
    delay(step_delay);
    digitalWrite(BUTTON_1_LED_PIN, LOW);
    delay(step_delay);
    digitalWrite(BUTTON_2_LED_PIN, LOW);
    delay(step_delay);
    digitalWrite(BUTTON_3_LED_PIN, LOW);
    delay(step_delay);
    digitalWrite(BUTTON_4_LED_PIN, LOW);
    delay(step_delay);
    digitalWrite(BUTTON_5_LED_PIN, LOW);
    delay(step_delay);
}

