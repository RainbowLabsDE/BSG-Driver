#include "Message.hpp"

class Msg_01F_Ctrl1 : public Message {
  public:
    class Mode : public Signal {
      public:
        Mode(uint8_t *msgData) : Signal(msgData, "Mode", 12, 4) {}
        enum {
            IDLE, // ?
            RUN,  // ?
        };
    };

    std::vector<Signal> signals = {
        Mode(data),
        Signal(data, "RPM Request", 16, 12, 2),
        Signal(data, 32, 11, 1024), // Unknown Data
    };
};