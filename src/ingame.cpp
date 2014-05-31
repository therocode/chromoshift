#include "ingame.hpp"

InGameState::InGameState(fea::MessageBus& bus) : 
    mBus(bus),
    mScene(bus)
{
    mBus.addSubscriber<QuitMessage>(*this);
}

InGameState::~InGameState()
{
    mBus.removeSubscriber<QuitMessage>(*this);
}

void InGameState::setup()
{
}

void InGameState::activate(const std::string& previous)
{
}

std::string InGameState::run()
{
    return mNextState;
}

void InGameState::handleMessage(const QuitMessage& message)
{
    mNextState = "NONE";
}
