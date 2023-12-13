#pragma once

#include "Message.hpp"
#include "E2E_CRC.hpp"

class M0AF_Transm2 : public Message {
    public:
    M0AF_Transm2() : Message(0x0AF, 8, 20, true) {}

    // Signals automatically register themselves in the "_signals" vector.
    SigCRC sCRC = SigCRC(this);
    SigSEQ sSEQ = SigSEQ(this);
    Signal sUnk1 = Signal(this, 37, 1, 1);

    const uint8_t _crcDataIdList[16] = { 246, 18, 41, 64, 87, 110, 133, 156, 179, 202, 225, 248, 20, 43, 66, 89 };
};