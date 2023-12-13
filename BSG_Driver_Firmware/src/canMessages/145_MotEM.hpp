#pragma once

#include "Message.hpp"
#include "E2E_CRC.hpp"

class M145_MotEM : public Message {
    public:
    M145_MotEM() : Message(0x145, 8, 20, true) {}

    // Signals automatically register themselves in the "_signals" vector.
    SigCRC sCRC = SigCRC(this);
    SigSEQ sSEQ = SigSEQ(this);
    Signal sUnk1 = Signal(this, 16, 13, 2000);
    Signal sUnk2 = Signal(this, 48, 13, 2000);

    const uint8_t* dataIdList() { return _crcDataIdList; }
    static inline constexpr const uint8_t _crcDataIdList[16] = { 171, 219, 16, 64, 112, 160, 208, 5, 53, 101, 149, 197, 245, 42, 90, 138 };


};