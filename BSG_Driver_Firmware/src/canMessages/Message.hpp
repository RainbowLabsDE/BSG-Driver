#pragma once

#include "Signal.hpp"
#include <stdint.h>
#include <vector>

class Message {
    public:
    Message(uint32_t id, uint8_t dataLength, int intervalTime = 0, bool hasCrc = false, bool isRxMsg = false);

    uint8_t* data() { return this->_data; }
    virtual const uint8_t* dataIdList() = 0;

    void registerSignal(Signal* sig) {
        _signals.push_back(sig);
    }

    std::vector<Signal*> _signals;
    const uint8_t _dataLength;
    uint8_t _data[8] = {0};

    const uint32_t _id;
    const int _intervalTime;
    const bool _hasCrc;             // if true, calculate CRC and SEQ signals before sending
    uint32_t _lastSentTime = 0;

};