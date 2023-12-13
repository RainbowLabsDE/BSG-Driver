#pragma once

#include "Message.hpp"
#include "E2E_CRC.hpp"

class M025_Ctrl2 : public Message {
    public:
    M025_Ctrl2() : Message(0x025, 8, 10, true) {}

    // Signals automatically register themselves in the "_signals" vector.
    SigCRC sCRC = SigCRC(this);
    SigSEQ sSEQ = SigSEQ(this);
    Signal sMode = Signal(this, "Mode", 12, 4, 1.0, 0.0, Mode::RUN);
    Signal sUnk1 = Signal(this, 16, 15, 16384);
    Signal sUnk2 = Signal(this, 32, 13, 4096);

    const uint8_t* dataIdList() { return _crcDataIdList; }
    static inline constexpr const uint8_t _crcDataIdList[16] = { 175, 186, 197, 208, 219, 230, 241, 252, 12, 23, 34, 45, 56, 67, 78, 89 };

    enum Mode {
        OFF = 0,
        BOOST = 4,
        RUN = 7,
    };
};