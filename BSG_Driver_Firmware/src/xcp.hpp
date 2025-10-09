#pragma once
#include <Arduino.h>
#include "can.h"

// Code that handles the XCP communication with the ECU
// Also implemented the rudimentary UDS communication in here
// and the state machine to do the FBS4 unlocking
// Could probably be structured a bit better, but eh

class XCP {
    protected:
    const int MAX_RETRIES = 4;
    const int RETRY_DELAY = 250;

    static inline constexpr uint8_t positiveUDSResponse[] = {0x06, 0x50, 0x03, 0x00};


    typedef struct {
        uint8_t fwVer[3];       // Expected firmware version number, patch only works with this version (Year, Week, Patch)
        uint32_t patchAddr;     // Address of FBS4 "personalized & activated" flag in memory
    } xcpPatchAddr_t;

    static inline constexpr xcpPatchAddr_t XCP_FBS4_PATCH_ADDR[] = {
        {{18, 20, 2}, 0x4000ecd6},  // Firmware version found in A2649001000
        {{21, 16, 1}, 0x4000eeea},  // Firmware version found in A2649001700
    };
    const uint32_t XCP_ADDR_FW_VER = 0x00014000;    // Address of firmware version in flash


    public:

    static const uint32_t CAN_ID_XCP_REQ = 0x18EF6201;
    static const uint32_t CAN_ID_XCP_RESP = 0x18EF6202;
    static const uint32_t CAN_ID_UDS_REQ = 0x7E6;
    static const uint32_t CAN_ID_UDS_RESP = 0x7EE;

    void udsEnterExtendedDiagnostic() {
        uint8_t payload[] = {0x02, 0x10, 0x03, 0, 0, 0, 0, 0};
        CANSendSimple(CAN_ID_UDS_REQ, payload, 8);
    }

    void udsHardReset() {
        uint8_t payload[] = {0x02, 0x11, 0x01, 0, 0, 0, 0, 0};
        CANSendSimple(CAN_ID_UDS_REQ, payload, 8);
    }

    void connect() {
        uint8_t payload[] = {(uint8_t)CMD::CONNECT, 0, 0, 0, 0, 0, 0, 0};
        CANSendSimple(CAN_ID_XCP_REQ, payload, 8);
    }

    void disconnect() {
        uint8_t payload[] = {(uint8_t)CMD::DISCONNECT, 0, 0, 0, 0, 0, 0, 0};
        CANSendSimple(CAN_ID_XCP_REQ, payload, 8);
    }

    // seek to address
    void set_mta(uint32_t address) {
        uint8_t payload[] = {(uint8_t)CMD::SET_MTA, 0, 0, 0, 0, 0, 0, 0};
        uint32_t addr_be = __ntohl(address);
        memcpy(payload + 4, &addr_be, 4);
        CANSendSimple(CAN_ID_XCP_REQ, payload, 8);
    }

    // push data to ECU
    // currently only does 6 bytes max
    void download(uint8_t *data, int len) {
        if (len <= 6) {
            uint8_t payload[] = {(uint8_t)CMD::DOWNLOAD, (uint8_t)len, 0, 0, 0, 0, 0, 0};
            memcpy(payload + 2, data, min(len, 6));
            CANSendSimple(CAN_ID_XCP_REQ, payload, 8);
        }
    }

    // get data data from ECU
    // currently only does 7 bytes max
    // TODO: make data available from externally? 
    void upload(int len) {
        if (len <= 7) {
            uint8_t payload[] = {(uint8_t)CMD::UPLOAD, (uint8_t)len, 0, 0, 0, 0, 0, 0};
            CANSendSimple(CAN_ID_XCP_REQ, payload, 8);
        }
    }

    // Start the process of deactivating FBS4
    void deactivateFBS4() {
        _state = State::EnterDiag;
        _retries = 0;
    }

    bool deactivatingDone() {
        return _state == State::PatchComplete;
    }

    // Handle incoming CAN messages (responses) by putting them in a buffer and setting the newMsg flag
    void handleMsg(CAN_msg_t &msg) {
        if (msg.id == CAN_ID_XCP_RESP || msg.id == CAN_ID_UDS_RESP) {
            if (_newMsg) {
                printf("Warning: XCP RX msg buffer still full!\n");
            }
            else {
                memcpy(&_rxMsg, &msg, sizeof(CAN_msg_t));
                _newMsg = true;
            }
        }
    }

    // Call this loop continuously, at least once after receiving a CAN message
    void loop () {
        bool stepSuccessful = false;

        if (_state == State::None || _state == State::PatchComplete || _state == State::Error) {
            // nothing to do
            return;
        }

        // handle responses
        if (_newMsg) {
            if (_rxMsg.id == CAN_ID_UDS_RESP) {         // UDS
                if (_state == State::EnterDiag) {
                    if (memcmp(_rxMsg.data, positiveUDSResponse, sizeof(positiveUDSResponse)) == 0) {   // is positive response
                        _state = State::Connect;
                        // stepSuccessful = true;   //?
                        _retries = 0;               // mark current step successful by resetting retry counter
                    }
                }
            }
            else if (_rxMsg.id == CAN_ID_XCP_RESP) {    // XCP
                switch (_state) {
                    case State::Connect: 
                    case State::UL_setAddr:
                    case State::DL_setAddr:
                    case State::DL_FBS4Patch:
                    case State::Disconnect:
                        if (_rxMsg.data[0] == (uint8_t)PID::RES) {  // successful XCP response
                            _state = (State)((int)_state + 1);  // go to next state (sadly a bit ugly due to enum class)
                            _retries = 0;                       // mark current step successful by resetting retry counter
                        }
                        break;
                    case State::UL_FWVer:
                        if (_rxMsg.data[0] == (uint8_t)PID::RES) {  // successful XCP response
                            uint8_t *readFwVersion = _rxMsg.data + 1;
                            _addrToPatch = UINT32_MAX;  // reset to invalid 

                            // Find matching firmware version
                            for (auto fwPatchAddr : XCP_FBS4_PATCH_ADDR) {
                                if (memcmp(readFwVersion, fwPatchAddr.fwVer, sizeof(xcpPatchAddr_t::fwVer)) == 0) {
                                    _addrToPatch = fwPatchAddr.patchAddr;
                                    _state = State::DL_setAddr;
                                    _retries = 0;       // mark current step successful by resetting retry counter
                                    break;
                                }
                            }

                            if (_addrToPatch == UINT32_MAX) {   // No matching firmware version found
                                printf("ERROR: possible incompatible firmware version detected (%02d.%02d.%02d), aborting FBS4 patch. \nPlease open an issue with firmware dump, so compatibility can be checked.\n", 
                                    _rxMsg.data[1], _rxMsg.data[2], _rxMsg.data[3]);
                                _state = State::Error;
                            }
                        }
                        break;
                }
            }
            _newMsg = false;    // mark message as handled
        }

        // Check for retries here 
        // (so it has one last chance to get handled and go to the next step, but doesn't trigger a spurious retry)
        if (_retries >= MAX_RETRIES) {
            printf("ERROR: Failed in XCP state machine, step: %d\n", (int)_state);
            _state = State::Error;
        }

        // actually do the current step
        if (_retries == 0 || (_retries < MAX_RETRIES && millis() - _lastStepTime > RETRY_DELAY)) {
            switch (_state) {
                case State::EnterDiag:
                    udsEnterExtendedDiagnostic();
                    break;
                case State::Connect:
                    connect();
                    break;
                case State::UL_setAddr:
                    set_mta(XCP_ADDR_FW_VER);
                    break;
                case State::UL_FWVer:
                    upload(3);
                    break;
                case State::DL_setAddr:
                    set_mta(_addrToPatch);
                    break;
                case State::DL_FBS4Patch: {
                    uint8_t payload = 1;
                    download(&payload, 1);
                    break;
                }
                case State::Disconnect:
                    disconnect();
                    break;
            }
            _retries++;
            _lastStepTime = millis();
        }
    }


    protected:

    enum class CMD : uint8_t {
        CONNECT = 0xFF,
        DISCONNECT = 0xFE,
        SET_MTA = 0xF6,
        UPLOAD = 0xF5,
        DOWNLOAD = 0xF0,
    };
    
    enum class PID : uint8_t {
        RES = 0xFF,
        ERR = 0xFE,
        EV = 0xFD,
        SERV = 0xFC,
    };

    enum class State {
        None,
        EnterDiag,
        Connect,
        UL_setAddr,
        UL_FWVer,
        DL_setAddr,
        DL_FBS4Patch,
        Disconnect,
        PatchComplete,
        Error,
    };

    State _state = State::None;
    uint32_t _lastStepTime;
    int _retries = 0;
    CAN_msg_t _rxMsg;
    bool _newMsg;
    uint32_t _addrToPatch = UINT32_MAX;
};

inline XCP xcp;