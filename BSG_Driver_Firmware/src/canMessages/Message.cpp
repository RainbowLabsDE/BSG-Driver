#include "Message.hpp"
#include "../CanSender.hpp"
#include "util.h"

// hacky way to register Message, TODO: improve
extern CanSender canSender;

Message::Message(uint32_t id, uint8_t dataLength, int intervalTime, bool hasCrc, bool isRxMsg)
    : _id(id), _dataLength(dataLength), _intervalTime(intervalTime), _hasCrc(hasCrc) {
    
    if (intervalTime != 0) {
        canSender.registerMessage(this);
    }

    if (isRxMsg) {
        canSender.registerRxMessage(this);
    }
}

void Message::printSignals() {
    printf("[%03X] ", this->_id);

    for (auto &sig : _signals) {
        const char *name = sig->_name;
        if (name && strcmp(name, "CRC") != 0 && strcmp(name, "SEQ") != 0) { // don't print CRC/SEQ
            printf("%s: ", sig->_name);
            printFloat(sig->getVal(), sig->_numDigits, sig->_numDecimals);
            printf("%s, ", sig->_unit ? sig->_unit : "");
        }
    }
    printf("\b \n");
}