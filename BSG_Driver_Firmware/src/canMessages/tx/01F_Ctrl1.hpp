#pragma once

#include "../Message.hpp"
#include "../E2E_CRC.hpp"

class M01F_Ctrl1 : public Message {
    public:
    // Try implementing a new Signal type in-place, but didn't work as I wanted
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
    Signal sRpmReq = Signal(this, "RPM_Req", 16, 12, 2.0*3.0);                                  // RPM as seen from the BSG (engine RPM is around 3x slower, according to the belt ratio)
    Signal sUnk4 = Signal(this, 28, 4, 1);
    Signal sTrqMinRq = Signal(this, "MinTrq_Req", 32, 11, 1/6.0, -512.0/3.0, 0.0, "Nm");        // Torque as seen from the BSG (engine torque is around 3x higher)
    Signal sTrqMaxRq = Signal(this, "MaxTrq_Req", 45, 11, 1/6.0, -512.0/3.0, 0.0, "Nm");
    Signal sTrqSyncRq = Signal(this, "SyncTrq_Req", 56, 8, 1.0/3.0, -112.0/3.0, 0.0, "Nm");     // ?

    const uint8_t* dataIdList() { return _crcDataIdList; }
    static inline constexpr const uint8_t _crcDataIdList[16] = { 25, 172, 68, 215, 111, 7, 154, 50, 197, 93, 240, 136, 32, 179, 75, 222 };

    enum Mode {
        IDLE,
        TRQ = 1,
        RPM = 5,
    };

    static inline const char *ModeStr[] = {
        "IDLE",
        "TRQ",
        "2",
        "3",
        "4",
        "RPM",
        "6",
        "7",
        "8",
        "9",
        "10",
        "11",
        "12",
        "13",
        "14",
        "15",
    };
};