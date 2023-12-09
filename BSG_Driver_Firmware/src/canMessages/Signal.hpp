#include <stdint.h>

class Signal {
  public:
    Signal(uint8_t *msgData, const char *name, uint8_t startBit, uint8_t length, float factor = 1, float offset = 0, const char *unit = "")
        : messageData(msgData), name(name), startBit(startBit), length(length), factor(factor), offset(offset), unit(unit) {}

    // Unknown signal
    Signal(uint8_t *msgData, uint8_t startBit, uint8_t length, uint64_t unknownData)
        : messageData(msgData), startBit(startBit), length(length) {
            uint64_t mask = ((1 << length) - 1) << startBit;
            *(uint64_t*)messageData &= ~mask;
            *(uint64_t*)messageData |= unknownData << startBit;
        }

  private:
    const char *name;
    uint8_t startBit;
    uint8_t length; // in bits
    float factor = 1;
    float offset = 0;
    const char *unit = "";
    // float min;       // currently unused
    // float max;
    uint8_t *messageData;
};