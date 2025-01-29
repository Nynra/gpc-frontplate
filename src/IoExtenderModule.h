#include <Arduino.h>
#include <MCP23017.h>

/*
 * This class is meant to handle an I/O module with 16 digital inputs and 16 digital outputs.
 */
class IoExtenderModule
{
public:
    // Setup method
    void setup(int address, int switch1pin, int switch2pin, int switch3pin, int switch4pin, int switch5pin, int sled1pin, int sled2pin, int sled3pin, int sled4pin, int sled5pin, int rswitch1pin, int rswitch2pin, int rswitch3pin, int rswitch4pin, int rswitch5pin);

    // Getters
    bool getSwitchState(int switchNumber);
    int getActiveRswitch();

    // Updating methods
    void updateSwitches();
    void updateSwitchLeds();
    void updateRotarySwitch();
    void update();

private:
    // MCP23017 object
    MCP23017 mcp;
    // Pin definitions
    int SWITCH1_PIN, SWITCH2_PIN, SWITCH3_PIN, SWITCH4_PIN, SWITCH5_PIN;
    int SLED1_PIN, SLED2_PIN, SLED3_PIN, SLED4_PIN, SLED5_PIN;
    int RSWITCH1_PIN, RSWITCH2_PIN, RSWITCH3_PIN, RSWITCH4_PIN, RSWITCH5_PIN;

    // Switch states
    bool SWITCH1_STATE = false;
    bool SWITCH2_STATE = false;
    bool SWITCH3_STATE = false;
    bool SWITCH4_STATE = false;
    bool SWITCH5_STATE = false;
    // Rswitch states
    bool RSWITCH1_STATE = false;
    bool RSWITCH2_STATE = false;
    bool RSWITCH3_STATE = false;
    bool RSWITCH4_STATE = false;
    bool RSWITCH5_STATE = false;
    int ACTIVE_RSWITCH = 0;
};