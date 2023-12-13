#pragma once

#include "Message.hpp"
#include "E2E_CRC.hpp"

class M0AE_Transm : public Message {
    public:
    M0AE_Transm() : Message(0x0AE, 8, 20, true) {}

    // Signals automatically register themselves in the "_signals" vector.
    SigCRC sCRC = SigCRC(this);
    SigSEQ sSEQ = SigSEQ(this);
    Signal sGear = Signal(this, "Gear", 12, 4);
    Signal sLever = Signal(this, "Lever", 48, 3);
    Signal sGearTarget = Signal(this, "Gear Target", 51, 4); // ?
    Signal sTrqReq = Signal(this, "Torque Request", 56, 8); //?

    const uint8_t* dataIdList() { return _crcDataIdList; }
    static inline constexpr const uint8_t _crcDataIdList[16] = { 54, 199, 93, 238, 132, 26, 171, 65, 210, 104, 249, 143, 37, 182, 76, 221 };

    enum Gear {
        N, 
        D1, D2, D3, D4, D5, D6, D7, D8, D9,
        R = 11, R2,
        P,
        FREE
    };

    enum Lever {
        L_P, L_R, L_N, L_D = 4
    };

};