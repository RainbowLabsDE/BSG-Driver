// CAN code from https://github.com/nopnop2002/Arduino-STM32-CAN

#pragma once

#include <stdint.h>

/* Symbolic names for bit rate of CAN message                                */
typedef enum { CAN_50KBPS, CAN_100KBPS, CAN_125KBPS, CAN_250KBPS, CAN_500KBPS, CAN_1000KBPS } BITRATE;

/* Symbolic names for formats of CAN message                                 */
typedef enum { STANDARD_FORMAT = 0, EXTENDED_FORMAT } CAN_FORMAT;

/* Symbolic names for type of CAN message                                    */
typedef enum { DATA_FRAME = 0, REMOTE_FRAME } CAN_FRAME;

typedef struct {
    uint32_t id;     /* 29 bit identifier                               */
    uint8_t data[8]; /* Data field                                      */
    uint8_t len;     /* Length of data field in bytes                   */
    uint8_t ch;      /* Object channel(Not use)                         */
    uint8_t format;  /* 0 - STANDARD, 1- EXTENDED IDENTIFIER            */
    uint8_t type;    /* 0 - DATA FRAME, 1 - REMOTE FRAME                */
} CAN_msg_t;

typedef const struct {
    uint8_t TS2;
    uint8_t TS1;
    uint8_t BRP;
} CAN_bit_timing_config_t;

bool CANInit(BITRATE bitrate, int remap);
void CANReceive(CAN_msg_t *CAN_rx_msg);
void CANSend(CAN_msg_t *CAN_tx_msg);
void CANSendSimple(uint32_t id, uint8_t *payload, uint8_t len, bool forceExtendedID = false);
uint8_t CANMsgAvail(void);