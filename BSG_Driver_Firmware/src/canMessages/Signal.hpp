#pragma once

#include <stdint.h>

class Signal {
  public:
    Signal(uint8_t *msgData, const char *name, uint8_t startBit, uint8_t length, float factor = 1, float offset = 0, const char *unit = "")
        : _messageData(msgData), _name(name), _startBit(startBit), _length(length), _factor(factor), _offset(offset), _unit(unit) {}

    // Unknown signal
    Signal(uint8_t *msgData, uint8_t startBit, uint8_t length, uint64_t unknownData)
        : _messageData(msgData), _startBit(startBit), _length(length) {
            setRaw(unknownData);
        }

    uint64_t getRaw() {
        uint64_t ret = *(uint64_t*)_messageData >> _startBit;
        ret &= (1 << _length) - 1;
        return ret;
    }

    void setRaw(uint64_t val) {
        uint64_t mask = ((1 << _length) - 1) << _startBit;
        *(uint64_t*)_messageData &= ~mask;
        *(uint64_t*)_messageData |= val << _startBit;
    };    

  private:
    const char* _name;
    const uint8_t _startBit;
    const uint8_t _length; // in bits
    const float _factor = 1;
    const float _offset = 0;
    const char* _unit = "";
    // float min;       // currently unused
    // float max;
    // const char* comment;

    uint8_t* _messageData;
};