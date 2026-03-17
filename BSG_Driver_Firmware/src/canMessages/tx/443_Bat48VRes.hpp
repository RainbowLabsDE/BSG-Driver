#pragma once

#include "../Message.hpp"
#include "../E2E_CRC.hpp"

class M443_Bat48VRes : public Message {
    public:
    M443_Bat48VRes() : Message(0x443, 8, 1000, false) {}
    const uint8_t* dataIdList() { return nullptr; }

    // Signals automatically register themselves in the "_signals" vector.
    Signal sResDis = Signal(this, "Int. Resistance Dischg", 0, 16, 0.001, 0.0, 0.01, "Ohm");    // minimum according to firmware: 0.001
    Signal sResChg = Signal(this, "Int. Resistance Chg", 32, 16, 0.001, 0.0, 0.01, "Ohm");      // minimum according to firmware: 0.001 (maybe 0.01-0.25 correlates to -250 - -10A chg current)
};