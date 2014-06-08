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
    mPlayerColour = message.colour;
}

void Interface::handleMessage(const GoalColourMessage& message)
{
    mGoalColour = message.colour;
}
