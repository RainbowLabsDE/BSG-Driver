#include "Message.hpp"
#include "../CanSender.hpp"

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
        if (strcmp(name, "CRC") != 0 && strcmp(name, "SEQ") != 0) { // don't print CRC/SEQ
            printf("%s: %*.*f%s, ", sig->_name, sig->_numDigits, sig->_numDecimals, sig->getVal(), sig->_unit ? sig->_unit : "");
        }
    }
    printf("\b \n");
}