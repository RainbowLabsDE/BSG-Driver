#pragma once

#include "Signal.hpp"
#include <stdint.h>
#include <vector>

class Message {
  public:
    Message(uint32_t id, uint8_t dataLength, int intervalTime = 0, bool hasCrc = false)
        : _id(id), _dataLength(dataLength), _intervalTime(intervalTime), _hasCrc(hasCrc) {}

    uint8_t* data() { return this->_data; }

    void registerSignal(Signal* sig) {
        _signals.push_back(sig);
    }

    std::vector<Signal*> _signals;
    const uint8_t _dataLength;
    const uint8_t _crcDataIdList[16] = {0};   // optional, used for CRC calculation 
    uint8_t _data[8];

    const uint32_t _id;
    const int _intervalTime;
    const bool _hasCrc;             // if true, calculate CRC and SEQ signals before sending
    uint32_t _lastSentTime = 0;

};