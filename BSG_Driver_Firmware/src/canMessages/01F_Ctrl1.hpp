#pragma once

#include "Message.hpp"
#include "E2E_CRC.hpp"

class Msg_01F_Ctrl1 : public Message {
  public:
    class Mode : public Signal {
      public:
        Mode(Message* msg) : Signal(msg, "Mode", 12, 4) {}
        enum {
            IDLE, // ?
            RUN,  // ?
        };
    };

    Msg_01F_Ctrl1() : Message(0x01F, 8, true) {}

    // Signals automatically register themselves in the "signals" vector.
    SigCRC sCRC = SigCRC(this);
    SigSEQ sSEQ = SigSEQ(this);
    Mode sMode = Mode(this);
    Signal sRpmReq = Signal(this, "RPM Request", 16, 12, 2.0);
    Signal sUnk1 = Signal(this, 32, 11, 1024); // Unknown Data

    const uint8_t _crcDataIdList[16] = { 25, 172, 68, 215, 111, 7, 154, 50, 197, 93, 240, 136, 32, 179, 75, 222 };
};