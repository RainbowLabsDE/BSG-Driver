#pragma once

#include "Message.hpp"
#include "E2E_CRC.hpp"

class M309_Info : public Message {
    public:
    M309_Info() : Message(0x309, 8, 1000, false) {}

    // Signals automatically register themselves in the "_signals" vector.
    Signal sOdo = Signal(this, "Odometer", 0, 24, 0.1, 0, "km");
    Signal sTemp = Signal(this, "Outside Temperature", 24, 8, 0.5, -40, "°C");
    Signal sSpeed = Signal(this, "Speed", 48, 12, 0.1, 0, "km/h");
};