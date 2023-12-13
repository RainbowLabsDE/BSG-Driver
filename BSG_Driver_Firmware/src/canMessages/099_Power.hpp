#pragma once

#include "Message.hpp"
#include "E2E_CRC.hpp"

class M099_Power : public Message {
  public:
    M099_Power() : Message(0x099, 8, 20, true) {}

    // Signals automatically register themselves in the "_signals" vector.
    SigCRC sCRC = SigCRC(this);
    SigSEQ sSEQ = SigSEQ(this);
    Signal sUnk1 = Signal(this, 16, 13, 2000);
    Signal sUnk2 = Signal(this, 32, 13, 2000);
    Signal sRecupAllow = Signal(this, "RecupAllowed", 45, 1);

    const uint8_t _crcDataIdList[16] = { 36, 16, 247, 227, 207, 187, 167, 147, 127, 107, 87, 67, 47, 27, 7, 238 };

};