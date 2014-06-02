#pragma once
#include <fea/structure.hpp>
#include "messages.hpp"
#include "scene.hpp"
#include "interface.hpp"
#include "renderer.hpp"
#include "levelloader.hpp"
#include "levelmanager.hpp"

class InGameState : 
    public fea::GameState,
    public QuitMessageReceiver,
    public LevelAdvanceMessageReceiver,
    public LevelRestartMessageReceiver,
    public LevelSolvedMessageReceiver,
    public PlayerDiedMessageReceiver
{
    public:
        InGameState(fea::MessageBus& bus, fea::Renderer2D& r);
        ~InGameState();
        void setup() override;
        void activate(const std::string& previous) override;
        std::string run() override;
        void handleMessage(const QuitMessage& message) override;
        void handleMessage(const LevelAdvanceMessage& message) override;
        void handleMessage(const LevelRestartMessage& message) override;
        void handleMessage(const LevelSolvedMessage& message) override;
        void handleMessage(const PlayerDiedMessage& message) override;
    private:
        void nextLevel();
        void previousLevel();
        void restartLevel();
        fea::MessageBus& mBus;
        Scene mScene;
        Interface mInterface;
        Renderer mRenderer;
        LevelLoader mLevelLoader;
        LevelManager mLevelManager;

        bool isDead;
        bool isSolved;

        std::string mNextState;
};
