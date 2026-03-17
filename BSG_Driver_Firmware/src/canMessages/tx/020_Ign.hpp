#pragma once

#include "../Message.hpp"
#include "../E2E_CRC.hpp"

class M020_Ignition : public Message {
    public:
    M020_Ignition() : Message(0x020, 4, 100, true) {}

    // Signals automatically register themselves in the "_signals" vector.
    SigCRC sCRC = SigCRC(this);
    SigSEQ sSEQ = SigSEQ(this);
    Signal sStatus = Signal(this, "Status", 12, 3);

    const uint8_t* dataIdList() { return _crcDataIdList; }
    static inline constexpr const uint8_t _crcDataIdList[16] = { 167, 211, 4, 48, 92, 136, 180, 224, 17, 61, 105, 149, 193, 237, 30, 74 };

    enum Status {
        OFF = 1,
        ON = 4,
        START = 5,
    };
};