#pragma once

#include <vector>
#include "canMessages/01F_Ctrl1.hpp"

class CanSender {
    public:
    CanSender() {}  // TODO: tx function

    void loop() {}  // TODO: send cyclic messages

    Msg_01F_Ctrl1 m01F_Ctrl1;

    // Do this for now, maybe I'll go for the self-registering approach in the future
    std::vector<Message*> messages = {
        &m01F_Ctrl1,
    };
};