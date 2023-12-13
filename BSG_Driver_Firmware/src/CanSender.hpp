#pragma once

#include <Arduino.h>
#include <vector>

#include "can.h"
#include "canMessages/01F_Ctrl1.hpp"
#include "canMessages/020_Ign.hpp"
#include "canMessages/025_Ctrl2.hpp"
#include "canMessages/081_MotorFoo.hpp"
#include "canMessages/099_Power.hpp"
#include "canMessages/0AE_Transmission.hpp"
#include "canMessages/0AF_Transmission2.hpp"
#include "canMessages/0B1_MotRPM.hpp"
#include "canMessages/0B9_MotSuppr.hpp"
#include "canMessages/145_MotEM.hpp"
#include "canMessages/170_OBD1.hpp"
#include "canMessages/173_OBD4.hpp"
#include "canMessages/2AF_Coolant.hpp"
#include "canMessages/2D3_Bat12V.hpp"
#include "canMessages/306_48VSwitch.hpp"
#include "canMessages/309_Info.hpp"
#include "canMessages/35A_Bat48V.hpp"
#include "canMessages/36F_Bat48VLim.hpp"
#include "canMessages/371_Bat48VPwr.hpp"
#include "canMessages/3CC_LowCool1.hpp"
#include "canMessages/3F2_LowCool2.hpp"
#include "canMessages/443_Bat48VRes.hpp"

class CanSender {
    public:
    CanSender(void (*canTxFunc)(CAN_msg_t*))
        : _canTx(canTxFunc) {} 

    // TODO: send cyclic messages
    void loop() {
        for (auto &msg : _messages) {
            if (msg->_intervalTime != 0 && millis() - msg->_lastSentTime >= msg->_intervalTime) {
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

    void registerMessage(Message* msg) {
        _messages.push_back(msg);
    }

    std::vector<Message*> _messages;    // Messages should register themselves
    void (*_canTx)(CAN_msg_t*);

    M01F_Ctrl1 m01F_Ctrl1;
    M020_Ignition m020_Ignition;
    M025_Ctrl2 m025_Ctrl2;
    M081_MotorFoo m081_MotorFoo;
    M099_Power m099_Power;
    M0AE_Transm m0AE_Transmission;
    M0AF_Transm2 m0AF_Transmission2;
    M0B1_MotRPM m0B1_MotRPM;
    M0B9_MotSuppr m0B9_MotSuppr;
    M145_MotEM m145_MotEM;
    M170_OBD1 m170_OBD1;
    M173_OBD4 m173_OBD4;
    M2AF_Coolant m2AF_Coolant;
    M2D3_Bat12V m2D3_Bat12V;
    M306_48VSwitch m306_48VSwitch;
    M309_Info m309_Info;
    M35A_Bat48V m35A_Bat48V;
    M36F_Bat48VLim m36F_Bat48VLim;
    M371_Bat48VPwr m371_Bat48VPwr;
    M3CC_LowCool1 m3CC_LowCool1;
    M3F2_LowCool2 m3F2_LowCool2;
    M443_Bat48VRes m443_Bat48VRes;
};