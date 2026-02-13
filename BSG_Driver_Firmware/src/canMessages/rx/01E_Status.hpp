#pragma once

#include "../Message.hpp"
#include "../E2E_CRC.hpp"

class M01E_Status : public Message {
    public:
    M01E_Status() : Message(0x01E, 8, 0, true, true) {}

    // Signals automatically register themselves in the "_signals" vector.
    SigCRC sCRC = SigCRC(this);
    SigSEQ sSEQ = SigSEQ(this);
    Signal sRPM = Signal(this, "RPM", 16, 14, 1.0, 0.0, 16383);                 // RPM
    Signal sTrq = Signal(this, "Trq", 48, 13, 0.25, -500, 1547.5, "Nm");        // Measured torque
    Signal sVolt = Signal(this, "DC_V", 32, 14, 0.1, 0.0, 1638.3, "V");         // Voltage of DC input
    Signal sFBS4 = Signal(this, "FBS4_actv", 46, 2);                            // FBS4 DAC active
    Signal sDir = Signal(this, "Dir", 30, 1);                                   // Direction, 0=CW, 1=CCW (viewed from front), but doesn't actually work
    Signal sLimit = Signal(this, "Limit", 61, 3);

    const uint8_t* dataIdList() { return _crcDataIdList; }
    static inline constexpr const uint8_t _crcDataIdList[16] = { 38, 180, 71, 213, 104, 246, 137, 28, 170, 61, 203, 94, 236, 127, 18, 160 };
};