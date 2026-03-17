#pragma once

#include "../Message.hpp"
#include "../E2E_CRC.hpp"

class M0B9_MotSuppr : public Message {
    public:
    M0B9_MotSuppr() : Message(0x0B9, 8, 20, true) {}

    // Signals automatically register themselves in the "_signals" vector.
    SigCRC sCRC = SigCRC(this);
    SigSEQ sSEQ = SigSEQ(this);
    Signal sStatus = Signal(this, "Status", 12, 3);
    Signal sUnk1 = Signal(this, 16, 13, 2000);
    Signal sUnk2 = Signal(this, 56, 7, 50);

    const uint8_t* dataIdList() { return _crcDataIdList; }
    static inline constexpr const uint8_t _crcDataIdList[16] = { 17, 14, 11, 8, 5, 253, 250, 247, 244, 241, 238, 235, 232, 229, 226, 223 };

    enum Status {
        STOP,
        START,
        RUN = 4,
    };

};