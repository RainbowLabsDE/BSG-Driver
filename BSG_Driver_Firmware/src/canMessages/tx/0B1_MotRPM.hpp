#pragma once

#include "../Message.hpp"
#include "../E2E_CRC.hpp"

class M0B1_MotRPM : public Message {
    public:
    M0B1_MotRPM() : Message(0x0B1, 8, 10, true) {}

    // Signals automatically register themselves in the "_signals" vector.
    SigCRC sCRC = SigCRC(this);
    SigSEQ sSEQ = SigSEQ(this);
    Signal sStatus = Signal(this, "Status", 12, 3);
    Signal sRPM = Signal(this, "RPM", 24, 14, 1.0, 0.0, 1000, "rpm");
    Signal sUnk1 = Signal(this, 40, 13, 2000);
    Signal sUnk2 = Signal(this, 56, 8, 0x37);

    const uint8_t* dataIdList() { return _crcDataIdList; }
    static inline constexpr const uint8_t _crcDataIdList[16] = { 199, 227, 4, 32, 60, 88, 116, 144, 172, 200, 228, 5, 33, 61, 89, 117 };

    enum Status {
        STOP,
        START,
        RUN = 4,
    };

};