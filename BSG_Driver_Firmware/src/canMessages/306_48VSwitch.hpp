#pragma once

#include "Message.hpp"
#include "E2E_CRC.hpp"

class M306_48VSwitch : public Message {
    public:
    M306_48VSwitch() : Message(0x306, 8, 200, false) {}

    // Signals automatically register themselves in the "_signals" vector.
    Signal sUnk1 = Signal(this, 58, 3, 2);
    Signal sUnk2 = Signal(this, 61, 3, 2);
};