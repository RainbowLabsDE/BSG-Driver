#pragma once

#include "../Message.hpp"
#include "../E2E_CRC.hpp"

class M35A_Bat48V : public Message {
    public:
    M35A_Bat48V() : Message(0x35A, 8, 100, false) {}
    const uint8_t* dataIdList() { return nullptr; }

    // Signals automatically register themselves in the "_signals" vector.
    Signal sVoltage = Signal(this, "Voltage", 0, 10, 0.1, 0, 48.0, "V");
    Signal sCurrent = Signal(this, "Current", 10, 10, 1, -500, 0.0, "A");
    Signal sCellMinV = Signal(this, "Min. Cell V", 20, 9, 0.01, 0.0, 3.7, "V");
    Signal sCellMaxV = Signal(this, "Max. Cell V", 29, 9, 0.01, 0.0, 3.7, "V");
};