#pragma once

#include "Message.hpp"
#include "E2E_CRC.hpp"

class M36F_Bat48VLim : public Message {
    public:
    M36F_Bat48VLim() : Message(0x36F, 8, 20, false) {}

    // Signals automatically register themselves in the "_signals" vector.
    Signal sUnk1 = Signal(this, (uint8_t)0, 8, 244);
    Signal sChgAMax = Signal(this, "Max. Charge Current", 8, 13, 0.1, -819, "A");
    Signal sChgVMax = Signal(this, "Max. Charge Voltage", 21, 13, 0.1, 0, "V");
    Signal sDisChgAMax = Signal(this, "Max. Discharge Current", 34, 13, 0.1, 0, "A");
    Signal sDisChgVMin = Signal(this, "Min. Discharge Voltage", 47, 13, 0.1, 0, "V");
};