#pragma once

#include "Message.hpp"
#include "E2E_CRC.hpp"

class M3CC_LowCool1 : public Message {
    public:
    M3CC_LowCool1() : Message(0x3CC, 8, 200, false) {}

    // Signals automatically register themselves in the "_signals" vector.
    Signal sCoolantTemp = Signal(this, "L.Coolant Temperature", 56, 8, 1.0, -40, "°C");
};