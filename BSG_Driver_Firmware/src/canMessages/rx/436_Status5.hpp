#pragma once

#include "../Message.hpp"
#include "../E2E_CRC.hpp"

class M436_Status5 : public Message {
    public:
    M436_Status5() : Message(0x436, 8, 0, false, true) {}
    const uint8_t* dataIdList() { return nullptr; }

    // Signals automatically register themselves in the "_signals" vector.
    Signal sCoolRqRotor = Signal(this, "CoolRqRotor", 24, 3, 0.5, 0.0, 3.5, "l/min");   // Rotor cooling request
    Signal sTempCoolOut = Signal(this, "T_Coolant", 32, 8, 1.0, -40, 215, "°C");        // Coolant exit temperator
    Signal sTempStator = Signal(this, "T_Stator", 40, 8, 1.0, -40, 215, "°C");          // Stator temperature
    Signal sTempInvrt = Signal(this, "T_Invrt", 48, 8, 1.0, -40, 215, "°C");            // Inverter temperature
    Signal sTempRot = Signal(this, "T_Rotor", 56, 8, 1.0, -40, 215, "°C");              // Rotor temperature
};