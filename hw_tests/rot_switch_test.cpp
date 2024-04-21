#include <Arduino.h>
#include <rot_switch.h>

const bool DEBUG = true;
const int BAUDRATE = 9600;
const unsigned int DELAY_TIME = 100;
const bool TEST_PINREADING = false;
const bool TEST_MOVEMENT_READING = true;
const bool TEST_ROT_SWITCH = false;

volatile int value1 = 0, value2 = 0, value3 = 0, value4 = 0, value5 = 0;

const int ROT_SWITCH_1_PIN = 12, ROT_SWITCH_2_PIN = 11, ROT_SWITCH_3_PIN = 10, ROT_SWITCH_4_PIN = 9, ROT_SWITCH_5_PIN = 8;
RotSwitch r_switch(ROT_SWITCH_1_PIN, ROT_SWITCH_2_PIN, ROT_SWITCH_3_PIN, ROT_SWITCH_4_PIN, ROT_SWITCH_5_PIN);

// Function prototypes
void test_pinreadings();
void test_movement_dir_readings();
void test_rot_switch();

void setup()
{
    Serial.begin(BAUDRATE);
    while (!Serial) { delay(10); }
    r_switch.setup();
    r_switch.setDebug(DEBUG);

    if (DEBUG)
    {
        Serial.println("Rotary switch test setup complete");
    }
}

void loop()
{
    r_switch.poll();
    if (TEST_PINREADING)
    {
        test_pinreadings();
    }
    if (TEST_MOVEMENT_READING)
    {
        test_movement_dir_readings();
    }
    if (TEST_ROT_SWITCH)
    {
        test_rot_switch();
    }
    delay(DELAY_TIME);
}


void test_pinreadings()
{
    value1 = digitalRead(ROT_SWITCH_1_PIN);
    value2 = digitalRead(ROT_SWITCH_2_PIN);
    value3 = digitalRead(ROT_SWITCH_3_PIN);
    value4 = digitalRead(ROT_SWITCH_4_PIN);
    value5 = digitalRead(ROT_SWITCH_5_PIN);

    // Only one should be high
    if (value1 + value2 + value3 + value4 + value5 != 1)
    {
        Serial.print("ERROR: More than one rotary switch pin is high: ");
        Serial.println(value1 + value2 + value3 + value4 + value5);
    } else if (value1 + value2 + value3 + value4 + value5 == 0)
    {
        Serial.println("ERROR: No rotary switch pin is high");
    } else {
        if (value1)
        {
            Serial.println("Rotary switch position: 1");
        } else if (value2)
        {
            Serial.println("Rotary switch position: 2");
        } else if (value3)
        {
            Serial.println("Rotary switch position: 3");
        } else if (value4)
        {
            Serial.println("Rotary switch position: 4");
        } else if (value5)
        {
            Serial.println("Rotary switch position: 5");
        }
    }
}

void test_movement_dir_readings()
{
    int change = r_switch.getChangeDirection();
    if (change != 0)
    {
        Serial.print("Rotary switch position: ");
        Serial.println(r_switch.getChangeDirection());
    }
}

void test_rot_switch()
{
    int pos = r_switch.getValue();
    if (pos != 0)
    {
        Serial.print("Rotary switch position: ");
        Serial.println(pos);
    }
}