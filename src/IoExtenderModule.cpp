#include <IoExtenderModule.h>
#include <MCP23017.h>

// Setup method
void IoExtenderModule::setup(int address, int switch1pin, int switch2pin, int switch3pin, int switch4pin, int switch5pin, int sled1pin, int sled2pin, int sled3pin, int sled4pin, int sled5pin, int rswitch1pin, int rswitch2pin, int rswitch3pin, int rswitch4pin, int rswitch5pin)
{
    // Set address
    this->mcp = MCP23017(address);

    // Set pin definitions
    this->SWITCH1_PIN = switch1pin;
    this->SWITCH2_PIN = switch2pin;
    this->SWITCH3_PIN = switch3pin;
    this->SWITCH4_PIN = switch4pin;
    this->SWITCH5_PIN = switch5pin;
    this->SLED1_PIN = sled1pin;
    this->SLED2_PIN = sled2pin;
    this->SLED3_PIN = sled3pin;
    this->SLED4_PIN = sled4pin;
    this->SLED5_PIN = sled5pin;
    this->RSWITCH1_PIN = rswitch1pin;
    this->RSWITCH2_PIN = rswitch2pin;
    this->RSWITCH3_PIN = rswitch3pin;
    this->RSWITCH4_PIN = rswitch4pin;
    this->RSWITCH5_PIN = rswitch5pin;

    // Initialize the MCP23017
    this->mcp.init();

    // Set pin modes
    this->mcp.pinMode(this->SWITCH1_PIN, INPUT, true);
    this->mcp.pinMode(this->SWITCH2_PIN, INPUT, true);
    this->mcp.pinMode(this->SWITCH3_PIN, INPUT, true);
    this->mcp.pinMode(this->SWITCH4_PIN, INPUT, true);
    this->mcp.pinMode(this->SWITCH5_PIN, INPUT, true);
    this->mcp.pinMode(this->SLED1_PIN, OUTPUT, false);
    this->mcp.pinMode(this->SLED2_PIN, OUTPUT, false);
    this->mcp.pinMode(this->SLED3_PIN, OUTPUT, false);
    this->mcp.pinMode(this->SLED4_PIN, OUTPUT, false);
    this->mcp.pinMode(this->SLED5_PIN, OUTPUT, false);
    this->mcp.pinMode(this->RSWITCH1_PIN, INPUT, true);
    this->mcp.pinMode(this->RSWITCH2_PIN, INPUT, true);
    this->mcp.pinMode(this->RSWITCH3_PIN, INPUT, true);
    this->mcp.pinMode(this->RSWITCH4_PIN, INPUT, true);
    this->mcp.pinMode(this->RSWITCH5_PIN, INPUT, true);
}

// Getters
bool IoExtenderModule::getSwitchState(int switchNumber)
{
    switch (switchNumber)
    {
    case 1:
        return this->SWITCH1_STATE;
        break;
    case 2:
        return this->SWITCH2_STATE;
        break;
    case 3:
        return this->SWITCH3_STATE;
        break;
    case 4:
        return this->SWITCH4_STATE;
        break;
    case 5:
        return this->SWITCH5_STATE;
        break;
    default:
        return false;
        break;
    }
}

int IoExtenderModule::getActiveRswitch()
{
    return this->ACTIVE_RSWITCH;
}

// Updating methods
void IoExtenderModule::updateSwitches()
{
    this->SWITCH1_STATE = this->mcp.digitalRead(this->SWITCH1_PIN);
    this->SWITCH2_STATE = this->mcp.digitalRead(this->SWITCH2_PIN);
    this->SWITCH3_STATE = this->mcp.digitalRead(this->SWITCH3_PIN);
    this->SWITCH4_STATE = this->mcp.digitalRead(this->SWITCH4_PIN);
    this->SWITCH5_STATE = this->mcp.digitalRead(this->SWITCH5_PIN);
}

void IoExtenderModule::updateSwitchLeds()
{
    this->mcp.digitalWrite(this->SLED1_PIN, this->SWITCH1_STATE);
    this->mcp.digitalWrite(this->SLED2_PIN, this->SWITCH2_STATE);
    this->mcp.digitalWrite(this->SLED3_PIN, this->SWITCH3_STATE);
    this->mcp.digitalWrite(this->SLED4_PIN, this->SWITCH4_STATE);
    this->mcp.digitalWrite(this->SLED5_PIN, this->SWITCH5_STATE);
}

void IoExtenderModule::updateRotarySwitch()
{
    this->RSWITCH1_STATE = this->mcp.digitalRead(this->RSWITCH1_PIN);
    this->RSWITCH2_STATE = this->mcp.digitalRead(this->RSWITCH2_PIN);
    this->RSWITCH3_STATE = this->mcp.digitalRead(this->RSWITCH3_PIN);
    this->RSWITCH4_STATE = this->mcp.digitalRead(this->RSWITCH4_PIN);
    this->RSWITCH5_STATE = this->mcp.digitalRead(this->RSWITCH5_PIN);

    if (this->RSWITCH1_STATE)
    {
        this->ACTIVE_RSWITCH = 1;
    }
    else if (this->RSWITCH2_STATE)
    {
        this->ACTIVE_RSWITCH = 2;
    }
    else if (this->RSWITCH3_STATE)
    {
        this->ACTIVE_RSWITCH = 3;
    }
    else if (this->RSWITCH4_STATE)
    {
        this->ACTIVE_RSWITCH = 4;
    }
    else if (this->RSWITCH5_STATE)
    {
        this->ACTIVE_RSWITCH = 5;
    }
}

void IoExtenderModule::update()
{
    this->updateSwitches();
    this->updateSwitchLeds();
    this->updateRotarySwitch();
}
