#include "ingame.hpp"

InGameState::InGameState(fea::MessageBus& bus, sf::RenderWindow& w) : 
    mBus(bus),
    mScene(bus),
    mInterface(bus),
    mRenderer(bus, w),
    mLevelLoader(bus),
    isSolved(false),
    isDead(false)
{
    mBus.addSubscriber<QuitMessage>(*this);
    mBus.addSubscriber<LevelAdvanceMessage>(*this);
    mBus.addSubscriber<LevelRestartMessage>(*this);
    mBus.addSubscriber<LevelSolvedMessage>(*this);
    mBus.addSubscriber<PlayerDiedMessage>(*this);
}

InGameState::~InGameState()
{
    mBus.removeSubscriber<QuitMessage>(*this);
    mBus.removeSubscriber<LevelAdvanceMessage>(*this);
    mBus.removeSubscriber<LevelRestartMessage>(*this);
    mBus.removeSubscriber<LevelSolvedMessage>(*this);
    mBus.removeSubscriber<PlayerDiedMessage>(*this);
}

void InGameState::setup()
{
    mLevelManager.loadLevelPack("levels/pack1");
}

void InGameState::activate(const std::string& previous)
{
    mLevelManager.reset();

    mLevelLoader.load(mLevelManager.same());
    mBus.send(SongPlayingMessage(true));
}

std::string InGameState::run()
{
    //if(isDead)
    //{
    //    restartLevel();
    //    isDead = false;
    //}
    if(isSolved)
    {
        nextLevel();
        isSolved = false;
    }

    mRenderer.render();
    return mNextState;
}

void InGameState::handleMessage(const QuitMessage& message)
{
    mNextState = "NONE";
}

void InGameState::handleMessage(const LevelAdvanceMessage& message)
{
    int32_t amount = std::get<0>(message.mData);

    if(amount > 0)
    {
        for(int32_t i = 0; i < amount; i++)
            nextLevel();
    }
    else if(amount < 0)
    {
        for(int32_t i = 0; i < abs(amount); i++)
            previousLevel();
    }
}

void InGameState::handleMessage(const LevelRestartMessage& message)
{
    restartLevel();
}

void InGameState::handleMessage(const LevelSolvedMessage& message)
{
    isSolved = true;
}

void InGameState::handleMessage(const PlayerDiedMessage& message)
{
    isDead = true;
}

void InGameState::nextLevel()
{
    if(mLevelManager.hasNext())
    {
        mLevelLoader.load(mLevelManager.next());
        mBus.send(SongPlayingMessage(true));
    }
}

void InGameState::previousLevel()
{
    if(mLevelManager.hasPrevious())
    {
        mLevelLoader.load(mLevelManager.previous());
        mBus.send(SongPlayingMessage(true));
    }
}

void InGameState::restartLevel()
{
    mLevelLoader.load(mLevelManager.same());
    mBus.send(SongPlayingMessage(true));
}
