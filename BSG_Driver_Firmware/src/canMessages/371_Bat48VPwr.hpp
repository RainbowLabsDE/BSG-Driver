#pragma once

#include "Message.hpp"
#include "E2E_CRC.hpp"

class M371_Bat48VPwr : public Message {
    public:
    M371_Bat48VPwr() : Message(0x371, 8, 20, true) {}

    // Signals automatically register themselves in the "_signals" vector.
    SigCRC sCRC = SigCRC(this);
    SigSEQ sSEQ = SigSEQ(this);
    Signal sMaxDisChgP = Signal(this, "Max. Discharge Power", 12, 14, 20, 0, 20e3, "W");
    Signal sChgV = Signal(this, "Charge Voltage", 26, 16, 0.02, 0, 50.4, "V");              // limit according to firmware: 30-52 (if I understood it correctly)
    Signal sMaxChgP = Signal(this, "Max. Charge Power", 42, 14, 20, 0, 20e3, "W");

    const uint8_t* dataIdList() { return _crcDataIdList; }
    static inline constexpr const uint8_t _crcDataIdList[16] = { 114, 178, 242, 55, 119, 183, 247, 60, 124, 188, 252, 65, 129, 193, 6, 70 };
};