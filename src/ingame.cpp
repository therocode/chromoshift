#include "ingame.hpp"

InGameState::InGameState(fea::MessageBus& b) : 
    bus(b)
{
    bus.addSubscriber<QuitMessage>(*this);
}

InGameState::~InGameState()
{
    bus.removeSubscriber<QuitMessage>(*this);
}

void InGameState::setup()
{
}

void InGameState::activate(const std::string& previous)
{
}

std::string InGameState::run()
{
    return nextState;
}

void InGameState::handleMessage(const QuitMessage& message)
{
    nextState = "NONE";
}
