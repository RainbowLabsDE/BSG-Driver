#include "Signal.hpp"
#include <stdint.h>
#include <vector>

class Message {
  public:
    std::vector<Signal> signals;

  protected:
    uint8_t data[8];
    int intervalTime;
    bool hasCrc; // if true, calculate CRC and SEQ signals before sending
};