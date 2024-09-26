#pragma once

#include "Message.hpp"
#include "E2E_CRC.hpp"

class M01F_Ctrl1 : public Message {
    public:
    // class Mode : public Signal {
    //   public:
    //     Mode(Message* msg) : Signal(msg, "Mode", 12, 4) {}
    //     enum {
    //         IDLE, // ?
    //         RUN,  // ?
    //     };
    // };

    M01F_Ctrl1() : Message(0x01F, 8, 10, true) {}

    // Signals automatically register themselves in the "_signals" vector.
    SigCRC sCRC = SigCRC(this);
    SigSEQ sSEQ = SigSEQ(this);
    // Mode sMode = Mode(this);
    Signal sMode = Signal(this, "Mode", 12, 4);
    Signal sRpmReq = Signal(this, "RPM Request", 16, 12, 2.0);  // RPM means "engine RPM", so BSG spins about 3x as fast (belt ratio)
    Signal sUnk4 = Signal(this, 28, 4, 1);
    Signal sTrqMinRq = Signal(this, "Min Torque Request", 32, 11, 0.5, -512, 0.0, "Nm");
    Signal sTrqMaxRq = Signal(this, "Max Torque Request", 45, 11, 0.5, -512, 0.0, "Nm");
    Signal sTrqSyncRq = Signal(this, "Sync? Torque Request", 56, 8, 1.0, -112, 0.0, "Nm");

    const uint8_t* dataIdList() { return _crcDataIdList; }
    static inline constexpr const uint8_t _crcDataIdList[16] = { 25, 172, 68, 215, 111, 7, 154, 50, 197, 93, 240, 136, 32, 179, 75, 222 };

    enum Mode {
        IDLE,
        TRQ = 1,
        RPM = 5,
    };
};