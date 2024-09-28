#pragma once

#include "../Message.hpp"
#include "../E2E_CRC.hpp"

class M190_Status2 : public Message {
    public:
    M190_Status2() : Message(0x190, 8, 0, true, true) {}

    // Signals automatically register themselves in the "_signals" vector.
    SigCRC sCRC = SigCRC(this);
    SigSEQ sSEQ = SigSEQ(this);
    Signal sState = Signal(this, "State", 29, 3);                                           // Current BSG state?
    Signal sPossTrqMin = Signal(this, "PossTrq_Min", 48, 13, 0.25, -1024, 1023.75, "Nm");   // Minimum possible torque
    Signal sPossTrqMax = Signal(this, "PossTrq_Max", 16, 13, 0.25, -1024, 1023.75, "Nm");   // Maximum possible torque
    Signal sCurNegTrq = Signal(this, "CurNegTrq", 32, 13, 0.25, -500, 1547.75, "Nm");       // Current negative torque
    Signal sUnkErr = Signal(this, "SomeErr", 45, 2);
    Signal sUnk = Signal(this, "Unk", 47, 1);

    const uint8_t* dataIdList() { return _crcDataIdList; }
    static inline constexpr const uint8_t _crcDataIdList[16] = { 205, 22, 90, 158, 226, 43, 111, 179, 247, 64, 132, 200, 17, 85, 153, 221 };

    static inline const char *StateStr[] = {
        "STANDBY",  // 0
        "IDLE",     // 1
        "RUNNING",  // 2
        "?",
        "?",
        "?",
        "FAULT",    // 6
        "?",
    };
};