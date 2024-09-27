#pragma once
#include <Arduino.h>
#include "can.h"

static const uint32_t CAN_ID_XCP_REQ = 0x18EF6201;
static const uint32_t CAN_ID_XCP_RESP = 0x18EF6202;
static const uint32_t CAN_ID_UDS_REQ = 0x7E6;

class XCP {
    public:
    void udsEnterExtendedDiagnostic() {
        CAN_msg_t msg = {
            .id = CAN_ID_UDS_REQ,
            .data = {0x02, 0x10, 0x03, 0, 0, 0, 0, 0},
            .len = 8,
        };
        CANSend(&msg);
    }

    void connect() {
        CAN_msg_t msg = {
            .id = CAN_ID_XCP_REQ,
            .data = {0xFF, 0, 0, 0, 0, 0, 0, 0},
            .len = 1,
            .format = EXTENDED_FORMAT,
        };
        CANSend(&msg);
    }

    // seek
    void set_mta(uint32_t address) {
        CAN_msg_t msg = {
            .id = CAN_ID_XCP_REQ,
            .data = {0xF6, 0, 0, 0, 0, 0, 0, 0},
            .len = 8,
            .format = EXTENDED_FORMAT,
        };
        uint32_t addr_be = __ntohl(address);
        memcpy(msg.data + 4, &addr_be, 4);
        CANSend(&msg);
    }

    // currently only does 6 bytes max
    // push data to ECU
    void download(uint8_t *data, int len) {
        CAN_msg_t msg = {
            .id = CAN_ID_XCP_REQ,
            .data = {0xF0, len, 0, 0, 0, 0, 0, 0},
            .len = 8,
            .format = EXTENDED_FORMAT,
        };
        memcpy(msg.data + 2, data, min(len, 6));
        CANSend(&msg);
    }

    // most hacky way to implement this, but whatevs
    // TODO: make better
    void deactivateFBS4() {
        udsEnterExtendedDiagnostic();
        delay(500);
        udsEnterExtendedDiagnostic();   // send twice to make sure device is awake
        delay(100);
        connect();
        delay(10);
        set_mta(0x4000ecd6);
        delay(10);
        uint8_t data = 1;
        download(&data, 1);
    }

};

inline XCP xcp;