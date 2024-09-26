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