#include <Arduino.h>

// Buttons
const int BUTTON_1_PIN = 28;
const int BUTTON_2_PIN = 29;
const int BUTTON_3_PIN = 30;
const int BUTTON_4_PIN = 31;
const int BUTTON_5_PIN = 32;

bool BUTTON_1_STATE = false;
bool BUTTON_2_STATE = false;
bool BUTTON_3_STATE = false;
bool BUTTON_4_STATE = false;
bool BUTTON_5_STATE = false;

// Button LED's
const int BUTTON_1_LED_PIN = 11;
const int BUTTON_2_LED_PIN = 10;
const int BUTTON_3_LED_PIN = 9;
const int BUTTON_4_LED_PIN = 8;
const int BUTTON_5_LED_PIN = 7;

void setup() {
  Serial.begin(9600);

  // put your setup code here, to run once:
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  pinMode(BUTTON_3_PIN, INPUT_PULLUP);
  pinMode(BUTTON_4_PIN, INPUT_PULLUP);
  pinMode(BUTTON_5_PIN, INPUT_PULLUP);

  pinMode(BUTTON_1_LED_PIN, OUTPUT);
  pinMode(BUTTON_2_LED_PIN, OUTPUT);
  pinMode(BUTTON_3_LED_PIN, OUTPUT);
  pinMode(BUTTON_4_LED_PIN, OUTPUT);
  pinMode(BUTTON_5_LED_PIN, OUTPUT);

  Serial.println("Setup complete");
}

void loop() {
  // put your main code here, to run repeatedly:
  BUTTON_1_STATE = digitalRead(BUTTON_1_PIN);
  BUTTON_2_STATE = digitalRead(BUTTON_2_PIN);
  BUTTON_3_STATE = digitalRead(BUTTON_3_PIN);
  BUTTON_4_STATE = digitalRead(BUTTON_4_PIN);
  BUTTON_5_STATE = digitalRead(BUTTON_5_PIN);

  digitalWrite(BUTTON_1_LED_PIN, true);
  digitalWrite(BUTTON_2_LED_PIN, true);
  digitalWrite(BUTTON_3_LED_PIN, true);
  digitalWrite(BUTTON_4_LED_PIN, true);
  digitalWrite(BUTTON_5_LED_PIN, true);

  Serial.print("Button 1: ");
  Serial.println(BUTTON_1_STATE);
  Serial.print("Button 2: ");
  Serial.println(BUTTON_2_STATE);
  Serial.print("Button 3: ");
  Serial.println(BUTTON_3_STATE);
  Serial.print("Button 4: ");
  Serial.println(BUTTON_4_STATE);
  Serial.print("Button 5: ");
  Serial.println(BUTTON_5_STATE);

  delay(1000);
}