#pragma once

#include "Message.hpp"
#include "E2E_CRC.hpp"

class M3F2_LowCool2 : public Message {
    public:
    M3F2_LowCool2() : Message(0x3F2, 8, 200, false) {}
    const uint8_t* dataIdList() { return nullptr; }

    // Signals automatically register themselves in the "_signals" vector.
    Signal sFlow = Signal(this, "Flow", 0, 9, 0.1, 0.0, 50.0, "l/min");
    Signal sTemp = Signal(this, "Temperature", 56, 8, 1.0, -40.0, 20.0, "°C");
};