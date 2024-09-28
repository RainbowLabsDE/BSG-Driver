#include <Arduino.h>

#include "can.h"
#include "util.h"
#include "CanSender.hpp"
#include "xcp.hpp"

CanSender canSender(CANSend);
bool started = false;


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

void initBSG() {
    xcp.deactivateFBS4();   // TODO: clean up internally

    delay(10);

    canSender.m01F_Ctrl1.sMode.setRaw(M01F_Ctrl1::Mode::IDLE);
    canSender.m020_Ignition.sStatus.setRaw(M020_Ignition::Status::ON);
    canSender.m0B1_MotRPM.sStatus.setRaw(M0B1_MotRPM::Status::RUN);

    canSender.m36F_Bat48VLim.sChgAMax.setVal(-50);
    canSender.m36F_Bat48VLim.sDisChgAMax.setVal(50);
    canSender.m36F_Bat48VLim.sChgVMax.setVal(54.0);
    canSender.m36F_Bat48VLim.sDisChgVMin.setVal(40.0);
    canSender.m443_Bat48VRes.sResChg.setVal(0.01);  // Ohm
    canSender.m443_Bat48VRes.sResDis.setVal(0.01);  // Ohm
    
}

void setup() {
    Serial.begin(115200);
    pinMode(PC13, OUTPUT);
    bool ret = CANInit(CAN_500KBPS, 2); // CAN_RX mapped to PB8, CAN_TX mapped to PB9
    if (!ret)
        while (true)
            ;
    
    delay(1000);

   
}

uint8_t counter = 0;
uint8_t frameLength = 0;
unsigned long previousMillis = 0; // stores last time output was updated
const long interval = 1000;       // transmission interval (milliseconds)


uint32_t prevDebug = 0;

int rpmRq = 0;
float trqMinRq = 0, trqMaxRq = 0;

void loop() {
    if (!started && millis() > 3000) {
        initBSG();
        started = true;
    }

    canSender.loop();   

    if (CANMsgAvail()) {
        CAN_msg_t msg = {0};
        CANReceive(&msg);
        handleMsg(msg);
    }

    while (Serial.available()) {
        // printf(" %02X ", Serial.read());
        char c = Serial.read();
        switch (c) {
            case 'i':
                canSender.m01F_Ctrl1.sMode.setRaw(M01F_Ctrl1::Mode::IDLE);
                // canSender.m020_Ignition.sStatus.setRaw(M020_Ignition::Status::ON);
                break;
            case 't':
                canSender.m01F_Ctrl1.sMode.setRaw(M01F_Ctrl1::Mode::TRQ);
                break;
            case 'r':
                canSender.m01F_Ctrl1.sMode.setRaw(M01F_Ctrl1::Mode::RPM);
                break;
            case 'q':   rpmRq += 2;         break;
            case 'Q':   rpmRq += 20;        break;
            case 'a':   rpmRq -= 2;         break;
            case 'A':   rpmRq -= 20;        break;
            case 'w':   trqMinRq += 0.5;    break;
            case 'W':   trqMinRq += 5;      break;
            case 's':   trqMinRq -= 0.5;    break;
            case 'S':   trqMinRq -= 5;      break;
            case 'e':   trqMaxRq += 0.5;    break;
            case 'E':   trqMaxRq += 5;      break;
            case 'd':   trqMaxRq -= 0.5;    break;
            case 'D':   trqMaxRq -= 5;      break;
            case ' ':
            case '\n':
            case '\r':
                rpmRq = 0;
                trqMinRq = 0;
                trqMaxRq = 0;
                canSender.m01F_Ctrl1.sMode.setRaw(M01F_Ctrl1::Mode::IDLE);
            break;
        }
        if (c >= '1' && c <= '9') {
            canSender.m01F_Ctrl1.sMode.setRaw(c - '0');
        }

        if (rpmRq < 0) rpmRq = 0;
        canSender.m01F_Ctrl1.sRpmReq.setVal(rpmRq); 

        canSender.m01F_Ctrl1.sTrqMinRq.setVal(trqMinRq);
        canSender.m01F_Ctrl1.sTrqMaxRq.setVal(trqMaxRq);
    }

    if (millis() - prevDebug >= 500) {
        prevDebug = millis();
        digitalWrite(PC13, !digitalRead(PC13));

        Serial.println();
        Serial.println(millis());
        if (millis() - canSender.m01E_Status._lastSentTime > 500) {
            printf("TIMEOUT!\n");
        }
        else {
            canSender.printRxMsgs();
            printf("REQUEST: Mode: %5s, RPM: %4g, TRQ_min: %5.1f, TRQ_max: %5.1f\n", 
                M01F_Ctrl1::ModeStr[canSender.m01F_Ctrl1.sMode.getRaw()], 
                canSender.m01F_Ctrl1.sRpmReq.getVal(),
                canSender.m01F_Ctrl1.sTrqMinRq.getVal(),
                canSender.m01F_Ctrl1.sTrqMaxRq.getVal()
                );
        }
    }

}
