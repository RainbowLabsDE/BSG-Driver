#include <Arduino.h>

#include "can.h"
#include "util.h"
#include "CanSender.hpp"

CanSender canSender(CANSend);


static const uint CAN_ID_XCP_REQ = 0x18EF6201;
static const uint CAN_ID_XCP_RESP = 0x18EF6202;


// TODO: move somewhere better
void handleMsg(CAN_msg_t& msg) {
    switch (msg.id) {
        case CAN_ID_XCP_RESP:
            // TODO
            break;
        default:
            bool knownMsg = canSender.handleMsg(msg);
            break;
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(PC13, OUTPUT);
    bool ret = CANInit(CAN_500KBPS, 2); // CAN_RX mapped to PB8, CAN_TX mapped to PB9
    if (!ret)
        while (true)
            ;
    
    delay(1000);

    // printHex(canSender.m01F_Ctrl1._crcDataIdList, 16);
    canSender.m01F_Ctrl1.sMode.setRaw(M01F_Ctrl1::Mode::RPM);
    canSender.m01F_Ctrl1.sTrqMinRq.setVal(0);
    canSender.m01F_Ctrl1.sTrqMaxRq.setVal(20);
    canSender.m01F_Ctrl1.sRpmReq.setVal(2); // 500 RPM == 1460 RPM
    canSender.m020_Ignition.sStatus.setRaw(M020_Ignition::Status::ON);
    canSender.m0B1_MotRPM.sStatus.setRaw(M0B1_MotRPM::Status::RUN);
    // printf("Msgs: %d\n", canSender._messages.size());

    // CAN_msg_t vinMsg = {.id = 0x071, .data = {0}, .len = 8};
    // CANSend(&vinMsg);
    // vinMsg.data[0]++;
    // CANSend(&vinMsg);
    // vinMsg.data[0]++;
    // CANSend(&vinMsg);

    // CAN_msg_t fbsMsg = {.id = 0x235, .data = {0}, .len = 8};
    // memset(fbsMsg.data, 0xFF, 8);
    // CANSend(&fbsMsg);
    
}

uint8_t counter = 0;
uint8_t frameLength = 0;
unsigned long previousMillis = 0; // stores last time output was updated
const long interval = 1000;       // transmission interval (milliseconds)


uint32_t prevDebug = 0;

void loop() {
    if (millis() - prevDebug >= 500) {
        prevDebug = millis();
        digitalWrite(PC13, !digitalRead(PC13));
        Serial.println(millis());
        // printf("[01E] RPM: %5.0f\n", canSender.m01E_Status.sRPM.getVal());
        // canSender.m01E_Status.printSignals();
        canSender.printRxMsgs();
        
        // printHex(canSender.m01F_Ctrl1.data(), 8);
        
        // printf("Signals: %d\n", canSender.m01F_Ctrl1._signals.size());
        // for (auto &sig : canSender.m01F_Ctrl1._signals) {
        //     if (sig->_name != nullptr) {
        //         printf("- %s\n", sig->_name);
        //     }
        //     else {
        //         printf("-\n");
        //     }
        // }
    }

    canSender.loop();   

    if (CANMsgAvail()) {
        CAN_msg_t msg = {0};
        CANReceive(&msg);
        handleMsg(msg);
    }
}
