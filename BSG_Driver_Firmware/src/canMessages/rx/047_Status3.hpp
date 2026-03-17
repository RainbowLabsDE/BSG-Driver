#pragma once

#include "../Message.hpp"
#include "../E2E_CRC.hpp"

class M047_Status3 : public Message {
    public:
    M047_Status3() : Message(0x047, 8, 0, false, true) {}
    const uint8_t* dataIdList() { return nullptr; }

    // Signals automatically register themselves in the "_signals" vector.
    Signal sReady = Signal(this, "Rdy", 14, 2);                                     // 1: not ready, 2: ready
    Signal sDCCur = Signal(this, "A_DC", 0, 14, 0.1, -819.2, 819.1, "A");           // DC Current
    Signal sPhaseCur = Signal(this, "A_Phase", 40, 13, 0.1, 0.0, 819.1, "A");       // Phase current
    Signal sLimitActive = Signal(this, "LimitActive", 16, 2);                       // Limit state
    Signal sLimitReason = Signal(this, "LimitReason", 36, 4);                       // Limit reason
    // Signal sUnkTrq = Signal(this, "Some torque?", 54, 10, 1.0, -1022, 0, "Nm");

    enum Ready {
        NOT_READY = 1,
        READY = 2,
    };

    enum LimitActive {
        NOLIM,
        LIM_CHG,
        LIM_DSCHG,
    };

    enum LimitReason {
        INACTIVE,
        VOLT_DC,
        CURR_DC,
        TRQ,
        SPD,
        INV,
        TEMP,
        R_UNKNOWN,
        PWR_,
        CURR_PH,
    };
};