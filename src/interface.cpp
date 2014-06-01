#include "interface.hpp"

Interface::Interface(fea::MessageBus& bus) :
    mBus(bus)
{
    mBus.addSubscriber<PlayerColourMessage>(*this);
    mBus.addSubscriber<GoalColourMessage>(*this);
}

Interface::~Interface()
{
    mBus.removeSubscriber<PlayerColourMessage>(*this);
    mBus.removeSubscriber<GoalColourMessage>(*this);
}

void Interface::handleMessage(const PlayerColourMessage& message)
{
    mPlayerColour = std::get<0>(message.mData);
}

void Interface::handleMessage(const GoalColourMessage& message)
{
    mGoalColour = std::get<0>(message.mData);
}
