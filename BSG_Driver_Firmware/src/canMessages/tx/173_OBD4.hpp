#pragma once

#include "../Message.hpp"
#include "../E2E_CRC.hpp"

class M173_OBD4 : public Message {
    public:
    M173_OBD4() : Message(0x173, 8, 100, false) {}
    const uint8_t* dataIdList() { return nullptr; }

    // Signals automatically register themselves in the "_signals" vector.
    Signal sPedal = Signal(this, "Pedal", 0, 8, 100.0/255.0);
    Signal sPedal2 = Signal(this, "Pedal2", 8, 8, 100.0/255.0);
};