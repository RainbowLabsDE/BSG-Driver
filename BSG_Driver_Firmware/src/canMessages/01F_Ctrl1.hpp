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
    Signal sRpmReq = Signal(this, "RPM Request", 16, 12, 2.0);
    Signal sUnk1 = Signal(this, 32, 11, 1024); // Unknown Data
    Signal sUnk2 = Signal(this, 45, 11, 1024); // Unknown Data
    Signal sUnk3 = Signal(this, 56, 8, 112); // Unknown Data

    const uint8_t* dataIdList() { return _crcDataIdList; }
    static inline constexpr const uint8_t _crcDataIdList[16] = { 25, 172, 68, 215, 111, 7, 154, 50, 197, 93, 240, 136, 32, 179, 75, 222 };

    enum Mode {
        IDLE, // ?
        RUN,  // ?
    };
};