#include "ingame.hpp"

InGameState::InGameState(fea::MessageBus& bus, sf::RenderWindow& w) : 
    mBus(bus),
    mScene(bus),
    mRenderer(bus, w),
    mLevelLoader(bus)
{
    mBus.addSubscriber<QuitMessage>(*this);
}

InGameState::~InGameState()
{
    mBus.removeSubscriber<QuitMessage>(*this);
}

void InGameState::setup()
{
    mLevelLoader.load("pack1/test");
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
