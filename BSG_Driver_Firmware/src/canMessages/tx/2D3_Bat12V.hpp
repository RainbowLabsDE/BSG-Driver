#pragma once

#include "../Message.hpp"
#include "../E2E_CRC.hpp"

class M2D3_Bat12V : public Message {
    public:
    M2D3_Bat12V() : Message(0x2D3, 8, 100, false) {}
    const uint8_t* dataIdList() { return nullptr; }

    // Signals automatically register themselves in the "_signals" vector.
    Signal sVoltage = Signal(this, "Voltage", 0, 8, 0.1, 0.0, 12.0, "V");
    Signal sCurrent = Signal(this, "Current", 8, 11, 0.1, -102.4, 0.5, "A");
    Signal sUnk1 = Signal(this, 21, 2, 1);
    Signal sUnk2 = Signal(this, 26, 2, 1);
};