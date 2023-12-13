#include "Message.hpp"
#include "../CanSender.hpp"

// hacky way to register Message, TODO: improve
extern CanSender canSender;

Message::Message(uint32_t id, uint8_t dataLength, int intervalTime, bool hasCrc)
    : _id(id), _dataLength(dataLength), _intervalTime(intervalTime), _hasCrc(hasCrc) {
    canSender.registerMessage(this);
}