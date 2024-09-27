#pragma once

#include "../Message.hpp"
#include "../E2E_CRC.hpp"

class M436_Status5 : public Message {
    public:
    M436_Status5() : Message(0x436, 8, 0, false, true) {}
    const uint8_t* dataIdList() { return nullptr; }

    // Signals automatically register themselves in the "_signals" vector.
    Signal sCoolRqRotor = Signal(this, "Cooling Request Rotor", 24, 3, 0.5, 0.0, 3.5, "l/min");
    Signal sTempCoolOut = Signal(this, "Temp. Coolant Exit", 32, 8, 1.0, -40, 215, "°C");
    Signal sTempStator = Signal(this, "Temp. Stator", 40, 8, 1.0, -40, 215, "°C");
    Signal sTempInvrt = Signal(this, "Temp. Inverter", 48, 8, 1.0, -40, 215, "°C");
    Signal sTempRot = Signal(this, "Temp. Rotor", 56, 8, 1.0, -40, 215, "°C");
};