#pragma once

#include <stdint.h>

class Message;  // forward declaration

class Signal {
  public:
    Signal(Message* msg, const char *name, uint8_t startBit, uint8_t length, float factor = 1.0, float offset = 0.0, float init = 0.0, const char *unit = nullptr);

    // Unknown signal
    Signal(Message* msg, uint8_t startBit, uint8_t length, uint64_t unknownData);

    uint64_t getRaw();
    void setRaw(uint64_t val);  
    float getVal();
    void setVal(float val);

    const char* _name;
    const uint8_t _startBit;
    const uint8_t _length; // in bits
    const float _factor = 1;
    const float _offset = 0;
    const char* _unit = nullptr;
    // float min;       // currently unused
    // float max;
    // const char* comment;

    
    // stuff needed for more elegant printing of values
    const int _numDigits = 0;
    const int _numDecimals = 0;
    constexpr int calcNum(uint8_t length, float factor, float offset, bool digitOrDecimal) const;


  protected:
    Message* _msg;
};