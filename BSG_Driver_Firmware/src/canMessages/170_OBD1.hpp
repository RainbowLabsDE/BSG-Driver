#pragma once

#include "Message.hpp"
#include "E2E_CRC.hpp"

class M170_OBD1 : public Message {
    public:
    M170_OBD1() : Message(0x170, 8, 100, false) {}

    // Signals automatically register themselves in the "_signals" vector.
    Signal sLoad = Signal(this, "Engine Load", 40, 8);
};