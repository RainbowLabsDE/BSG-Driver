#include <Arduino.h>

#include "can.h"
#include "util.h"
#include "CanSender.hpp"

CanSender canSender(CANSend);

void setup() {
    Serial.begin(115200);
    pinMode(PC13, OUTPUT);
    bool ret = CANInit(CAN_500KBPS, 2); // CAN_RX mapped to PB8, CAN_TX mapped to PB9
    if (!ret)
        while (true)
            ;
    
    delay(1000);

    // printHex(canSender.m01F_Ctrl1._crcDataIdList, 16);
    canSender.m01F_Ctrl1.sRpmReq.setRaw(0x10);
    canSender.m01F_Ctrl1.sMode.setRaw(M01F_Ctrl1::Mode::RUN);
    canSender.m01F_Ctrl1.sTrqMaxRq.setVal(1);
    canSender.m020_Ignition.sStatus.setRaw(M020_Ignition::Status::ON);
    canSender.m020_Ignition.sStatus2.setRaw(1);
    canSender.m020_Ignition.sStatus3.setRaw(1);
    canSender.m0B1_MotRPM.sStatus.setRaw(M0B1_MotRPM::Status::RUN);
    canSender.m0AE_Transmission.sGear.setRaw(M0AE_Transm::Gear::D1);
    canSender.m0AE_Transmission.sGearTarget.setRaw(M0AE_Transm::Gear::D1);
    canSender.m0AE_Transmission.sLever.setRaw(M0AE_Transm::Lever::L_D);
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
}

// void loopCanDemo() {
//     CAN_msg_t CAN_TX_msg;
//     CAN_msg_t CAN_RX_msg;

//     CAN_TX_msg.data[0] = 0x00;
//     CAN_TX_msg.data[1] = 0x01;
//     CAN_TX_msg.data[2] = 0x02;
//     CAN_TX_msg.data[3] = 0x03;
//     CAN_TX_msg.data[4] = 0x04;
//     CAN_TX_msg.data[5] = 0x05;
//     CAN_TX_msg.data[6] = 0x06;
//     CAN_TX_msg.data[7] = 0x07;
//     CAN_TX_msg.len = frameLength;

//     unsigned long currentMillis = millis();
//     if (currentMillis - previousMillis >= interval) {
//         previousMillis = currentMillis;
//         if ((counter % 2) == 0) {
//             CAN_TX_msg.type = DATA_FRAME;
//             if (CAN_TX_msg.len == 0)
//                 CAN_TX_msg.type = REMOTE_FRAME;
//             CAN_TX_msg.format = EXTENDED_FORMAT;
//             CAN_TX_msg.id = 0x32F072;
//         } else {
//             CAN_TX_msg.type = DATA_FRAME;
//             if (CAN_TX_msg.len == 0)
//                 CAN_TX_msg.type = REMOTE_FRAME;
//             CAN_TX_msg.format = STANDARD_FORMAT;
//             CAN_TX_msg.id = 0x072;
//         }
//         CANSend(&CAN_TX_msg);
//         frameLength++;
//         if (frameLength == 9)
//             frameLength = 0;
//         counter++;
//     }

//     if (CANMsgAvail()) {
//         CANReceive(&CAN_RX_msg);

//         if (CAN_RX_msg.format == EXTENDED_FORMAT) {
//             Serial.print("Extended ID: 0x");
//             if (CAN_RX_msg.id < 0x10000000)
//                 Serial.print("0");
//             if (CAN_RX_msg.id < 0x1000000)
//                 Serial.print("0");
//             if (CAN_RX_msg.id < 0x100000)
//                 Serial.print("0");
//             if (CAN_RX_msg.id < 0x10000)
//                 Serial.print("0");
//             if (CAN_RX_msg.id < 0x1000)
//                 Serial.print("0");
//             if (CAN_RX_msg.id < 0x100)
//                 Serial.print("0");
//             if (CAN_RX_msg.id < 0x10)
//                 Serial.print("0");
//             Serial.print(CAN_RX_msg.id, HEX);
//         } else {
//             Serial.print("Standard ID: 0x");
//             if (CAN_RX_msg.id < 0x100)
//                 Serial.print("0");
//             if (CAN_RX_msg.id < 0x10)
//                 Serial.print("0");
//             Serial.print(CAN_RX_msg.id, HEX);
//             Serial.print("     ");
//         }

//         Serial.print(" DLC: ");
//         Serial.print(CAN_RX_msg.len);
//         if (CAN_RX_msg.type == DATA_FRAME) {
//             Serial.print(" Data: ");
//             for (int i = 0; i < CAN_RX_msg.len; i++) {
//                 Serial.print("0x");
//                 Serial.print(CAN_RX_msg.data[i], HEX);
//                 if (i != (CAN_RX_msg.len - 1))
//                     Serial.print(" ");
//             }
//             Serial.println();
//         } else {
//             Serial.println(" Data: REMOTE REQUEST FRAME");
//         }
//     }
// }