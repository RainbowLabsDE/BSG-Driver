#pragma once

#include "Message.hpp"
#include "E2E_CRC.hpp"

class Msg_01F_Ctrl1 : public Message {
  public:
    class Mode : public Signal {
      public:
        Mode(uint8_t *msgData) : Signal(msgData, "Mode", 12, 4) {}
        enum {
            IDLE, // ?
            RUN,  // ?
        };
    };

    Msg_01F_Ctrl1() : Message(0x01F, 0, true) {}

    SigCRC sCRC = SigCRC(_data);
    SigSEQ sSEQ = SigSEQ(_data);
    Mode sMode = Mode(_data);
    Signal sRpmReq = Signal(_data, "RPM Request", 16, 12, 2.0);
    Signal sUnk1 = Signal(_data, 32, 11, 1024); // Unknown Data

    std::vector<Signal*> signals = {
        &sCRC,
        &sSEQ,
        &sMode,
        &sRpmReq,
        &sUnk1
    };

    const uint8_t _crcDataIdList[16] = { 25, 172, 68, 215, 111, 7, 154, 50, 197, 93, 240, 136, 32, 179, 75, 222 };
};