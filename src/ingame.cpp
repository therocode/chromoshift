#include "ingame.hpp"

InGameState::InGameState(fea::MessageBus& bus, sf::RenderWindow& w) : 
    mBus(bus),
    mScene(bus),
    mInterface(bus),
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
    mLevelManager.loadLevelPack("levels/pack1");
}

void InGameState::activate(const std::string& previous)
{
    mLevelManager.reset();
    nextLevel();
}

std::string InGameState::run()
{
    mRenderer.render();
    return mNextState;
}

void InGameState::handleMessage(const QuitMessage& message)
{
    mNextState = "NONE";
}

void InGameState::nextLevel()
{
    if(mLevelManager.hasNext())
    {
        mLevelLoader.load(mLevelManager.next());
        mBus.send(SongPlayingMessage(true));
    }
    else
    {
        //no more levels in the pack!
        mNextState = "NONE";
    }
}
