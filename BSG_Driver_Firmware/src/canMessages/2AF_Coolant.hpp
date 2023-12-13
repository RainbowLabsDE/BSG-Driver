#pragma once

#include "Message.hpp"
#include "E2E_CRC.hpp"

class M2AF_Coolant : public Message {
    public:
    M2AF_Coolant() : Message(0x2AF, 8, 100, false) {}

    // Signals automatically register themselves in the "_signals" vector.
    Signal sCoolantTemp = Signal(this, "Coolant Temperature", 0, 8, 1.0, -40.0, 20.0, "°C");
    Signal sUnkFlag = Signal(this, 8, 1, 0);
};