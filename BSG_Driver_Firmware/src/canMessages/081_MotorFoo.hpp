#pragma once

#include "Message.hpp"
#include "E2E_CRC.hpp"

class M081_MotorFoo : public Message {
    public:
    M081_MotorFoo() : Message(0x081, 2, 0, false) {}
    const uint8_t* dataIdList() { return nullptr; }

    // Signals automatically register themselves in the "_signals" vector.
    Signal sUnk1 = Signal(this, (uint8_t)0, 8, 0);
    Signal sUnk2 = Signal(this, 8, 8, 0);
};