#pragma once

#include "../Message.hpp"
#include "../E2E_CRC.hpp"

class M047_Status3 : public Message {
    public:
    M047_Status3() : Message(0x047, 8, 0, false, true) {}
    const uint8_t* dataIdList() { return nullptr; }

    // Signals automatically register themselves in the "_signals" vector.
    Signal sDCCur = Signal(this, "DC Current", 0, 14, 0.1, -819.2, 819.1, "A");
    Signal sReady = Signal(this, "Ready", 14, 2);   // 1: not ready, 2: ready
    Signal sPhaseCur = Signal(this, "Phase Current", 40, 13, 0.1, 0.0, 819.1, "A");
    Signal sUnkTrq = Signal(this, "Some torque?", 54, 10, 1.0, -1022, 0, "Nm");

    enum Ready {
        NOT_READY = 1,
        READY = 2,
    };
};