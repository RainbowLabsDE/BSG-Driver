#pragma once

#include <vector>
#include "can.h"
#include "canMessages/01F_Ctrl1.hpp"
#include "canMessages/020_Ign.hpp"

class CanSender {
    public:
    CanSender(void (*canTxFunc)(CAN_msg_t*))
        : _canTx(canTxFunc) {} 

    // TODO: send cyclic messages
    void loop() {
        for (auto &msg : _messages) {
            if (millis() - msg->_lastSentTime >= msg->_intervalTime) {
                msg->_lastSentTime = millis();

                if (msg->_hasCrc) {
                //if (msg->_signals.size() >= 2 && msg->_signals[0]->_name == "CRC" && msg->_signals[1]->_name == "SEQ") {
                    // calculate and store CRC + SEQ
                    E2E_P02::protect(msg);
                }

                CAN_msg_t canMsg = {
                    .id = msg->_id,
                    .len = msg->_dataLength,
                    .format = STANDARD_FORMAT,
                    .type = DATA_FRAME,
                };
                memcpy(canMsg.data, msg->data(), msg->_dataLength);

                _canTx(&canMsg);
            }
        }
    }  

    void (*_canTx)(CAN_msg_t*);

    M01F_Ctrl1 m01F_Ctrl1;
    M020_Ignition m020_Ignition;

    // Do this for now, maybe I'll go for the self-registering approach in the future
    std::vector<Message*> _messages = {
        &m01F_Ctrl1,
        &m020_Ignition,
    };
};