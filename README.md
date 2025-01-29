# GPC-frontplate

This repo contains the PlatformIO project used to control the interface on the frond side of the GPC (General Purpose Controller).
The front-plate code runs on an Arduino Nano.

## Draft menu tree

```plaintext
MainMenu
|
|- Info
|
|- Modes
|  |- DMM
|  |  |- Voltage
|  |- FunctionGenerator
|  |- DataLogger
|  |- RemoteControl
|
|- Settings
|  |- Safety
|  |- Display
```

## Layout

Consists of:

- 5 switches
- 5 switch LEDs
- 1 rotary switch (5 positions)
- 1 joystick (2 axis and a button)
- 1 potentiometer
- 1 temperature sensor
- 1 LCD display (20x4)
- 1 case fan
- 1 case intrusion switch
- 1 buzzer
- 1 Arduino Uno with Grove HAT
- 1 Grove I2C Bridge
- 1 MCP23017 I2C I/O Expander
- 1 Raspberry Pi 5 with Nvme Hat and DAQC2 HAT
- 1 esp32C3 with antenna

**Arduino**: Powered by barrel jack, usb is only for serial communication to
the RPI.

```plaintext
Arduin Grove HAT

- UART          -> NC
- D2 (d2)       -> Case Switch
- D3 (d3~)      -> Case Fan Speed
- D4 (d5~)      -> Buzzer
- D5 (d5, d6)   -> NC
- D6 (d6, d7)   -> NC
- D7 (d7, d8)   -> NC
- D8 (d8, d9)   -> NC
- I2C1 (a4, a5) -> NC
- I2C2 (a4, a5) -> NC
- I2C3 (a4, a5) -> NC
- I2C4 (a4, a5) -> NC
- A0 (a0)       -> Joystick X
- A1 (a1)       -> Joystick Y
- A2 (a2)       -> Potentiometer
- A3 (a3)       -> Temperature Sensor
```

**Grove I2C Bridge**: Used to connect Arduino to I2C clients

```plaintext
- I2C1 -> I2C1 Arduino
- I2C2 -> MCP23017
- I2C3 -> I2C LCD
- I2C4 -> NC
```

**MCP23017**: I/O Expander, should get power from the Arduino and connect to the I2C bridge and front panel hardware.

```plaintext
- A0 (addr)  -> NC
- A1 (addr)  -> NC
- A2 (addr)  -> NC
- Reset      -> NC
- NC/SO      -> NC
- NC/CS      -> NC
- SDA/SI     -> I2C2 I2C Bridge
- SCL/SCK    -> I2C2 I2C Bridge
- GND1       -> GND
- VCC1       -> 5V
- VCC2       -> NC
- INTB       -> NC
- B0         -> Switch 1
- B1         -> Switch 2
- B2         -> Switch 3
- B3         -> Switch 4
- B4         -> Switch 5
- B5         -> Switch LED 1
- B6         -> Switch LED 2
- B7         -> Switch LED 3
- GND2       -> GND
- INTA       -> Switch LED 4
- A0         -> Switch LED 5
- A1         -> Rotary Switch Pos 1
- A2         -> Rotary Switch Pos 2
- A3         -> Rotary Switch Pos 3
- A4         -> Rotary Switch Pos 4
- A5         -> Rotary Switch Pos 5
- A6         -> Joystick Button
- A7         -> NC
```
