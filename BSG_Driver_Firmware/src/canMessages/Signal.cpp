// Sadly I couldn't do the function declaration inside the .hpp, because of a cyclic dependency to Message. 
// (I resent duplicate code, like the function signature definition here, ugh)

#include "Signal.hpp"
#include "Message.hpp"
#include <string.h>

Signal::Signal(Message* msg, const char *name, uint8_t startBit, uint8_t length, float factor, float offset, float init, const char *unit)
    : _msg(msg), _name(name), _startBit(startBit), _length(length), _factor(factor), _offset(offset), _unit(unit) {
    setVal(init);
    _msg->registerSignal(this); // register myself in message (as to avoid duplicate code while defining signals in Message)
}

Signal::Signal(Message* msg, uint8_t startBit, uint8_t length, uint64_t unknownData)
    : _msg(msg), _startBit(startBit), _length(length) {
    setRaw(unknownData);
    _msg->registerSignal(this);
}

uint64_t Signal::getRaw() {
    uint64_t ret;
    memcpy(&ret, _msg->data(), sizeof(ret));
    ret >>= _startBit;
    ret &= (1 << _length) - 1;
    return ret;
}

void Signal::setRaw(uint64_t val) {
    uint64_t tmp;
    memcpy(&tmp, _msg->data(), sizeof(tmp));
    uint64_t mask = ((1 << _length) - 1) << _startBit;
    tmp &= ~mask;
    tmp |= val << _startBit;
    memcpy(_msg->data(), &tmp, sizeof(tmp));

    // for (int i = _startBit / 8; i < (_startBit + _length + 7) / 8; i++) {
    //     uint8_t bitPos = _startBit * 8;
    // }
};  

float Signal::getVal() {
    float val = (getRaw() * _factor) + _offset;
    return val;
}

void Signal::setVal(float val) {
    uint64_t raw = (val - _offset) / _factor;
    setRaw(raw);
}