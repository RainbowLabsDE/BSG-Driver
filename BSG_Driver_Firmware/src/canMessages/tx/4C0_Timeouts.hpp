#pragma once

#include "../Message.hpp"
#include "../E2E_CRC.hpp"

class M4C0_Timeouts : public Message {
    public:
    M4C0_Timeouts() : Message(0x4C0, 8, 1000, false) {}
    const uint8_t* dataIdList() { return nullptr; }

    // Signals automatically register themselves in the "_signals" vector.
    Signal tOut1 = Signal(this, "Timeout 1", 24, 12, 1.0, 0.0, 0.0, "min?");
    Signal tOut2 = Signal(this, "Timeout 2", 24, 12, 1.0, 0.0, 0.0, "min?");
};