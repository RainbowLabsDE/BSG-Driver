#include <Arduino.h>

#include "can.h"
#include "util.h"
#include "CanSender.hpp"
#include "xcp.hpp"

// initialize class that handles the periodic sending of the required control messages
CanSender canSender(CANSend);
bool started = false;

int rpmRq = 0;
float trqMinRq = 0, trqMaxRq = 0, trqSyncRq = 0;

void initBSG() {
    canSender.m01F_Ctrl1.sMode.setRaw(M01F_Ctrl1::Mode::IDLE);
    canSender.m020_Ignition.sStatus.setRaw(M020_Ignition::Status::ON);
    canSender.m0B1_MotRPM.sStatus.setRaw(M0B1_MotRPM::Status::RUN);

    canSender.m36F_Bat48VLim.sChgAMax.setVal(-260);
    canSender.m36F_Bat48VLim.sDisChgAMax.setVal(350);
    canSender.m36F_Bat48VLim.sChgVMax.setVal(54.0);
    canSender.m36F_Bat48VLim.sDisChgVMin.setVal(24.0);
    canSender.m443_Bat48VRes.sResChg.setVal(0.01);  // Ohm
    canSender.m443_Bat48VRes.sResDis.setVal(0.01);  // Ohm

    // // do something on startup (for MakerFaire)
    // trqMaxRq = 2;
    // rpmRq = 10;
    // canSender.m01F_Ctrl1.sTrqMaxRq.setVal(trqMaxRq);
    // canSender.m01F_Ctrl1.sRpmReq.setVal(rpmRq); 
    // canSender.m01F_Ctrl1.sMode.setRaw(M01F_Ctrl1::Mode::RPM);
}

void setup() {
    Serial.begin(115200);
    pinMode(PC13, OUTPUT);
    bool ret = CANInit(CAN_500KBPS, 2); // Remap 2: CAN_RX mapped to PB8, CAN_TX mapped to PB9
    if (!ret)
        while (true)
            ;
    
    delay(1000);

    initBSG();
}

uint8_t counter = 0;
uint8_t frameLength = 0;
unsigned long previousMillis = 0; // stores last time output was updated
const long interval = 1000;       // transmission interval (milliseconds)

uint32_t prevDebug = 0;

uint32_t _lastFBS4Unlock = 0;

void loop() {
    if (!started && millis() > 1000) {
        xcp.deactivateFBS4();
        started = true;
    }

    canSender.loop();   

    if (CANMsgAvail()) {
        CAN_msg_t msg = {0};
        CANReceive(&msg);

        xcp.handleMsg(msg);
        bool knownMsg = canSender.handleMsg(msg);
    }

    xcp.loop();

    // Redo FBS4 unlocking if it's locked for 3s (also retry every 3s)
    if (millis() - canSender.m01E_Status._lastSentTime < 100) { // BSG active and sending
        if (canSender.m01E_Status.sFBS4.getRaw() == 0) {    // reset timeout if status == ok
            _lastFBS4Unlock = millis();
        }
        else if (millis() - _lastFBS4Unlock > 3000) {       // unlock after timeout
            _lastFBS4Unlock = millis();
            xcp.deactivateFBS4();
        }

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
            case 'y':   trqSyncRq += 1;     break;
            case 'x':   trqSyncRq -= 1;     break;
            case ' ':
            case '\n':
            case '\r':
                rpmRq = 0;
                trqMinRq = 0;
                trqMaxRq = 0;
                trqSyncRq = 0;
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
        canSender.m01F_Ctrl1.sTrqSyncRq.setVal(trqSyncRq);
    }

    if (millis() - prevDebug >= 100) {
        prevDebug = millis();
        digitalWrite(PC13, !digitalRead(PC13));

        Serial.println();
        Serial.println(millis());
        if (millis() - canSender.m01E_Status._lastSentTime > 500) {
            printf("TIMEOUT!\n");
        }
        else {
            canSender.printRxMsgs();
            printf("REQUEST:\n");
            canSender.m01F_Ctrl1.printSignals();
        }
    }
}

