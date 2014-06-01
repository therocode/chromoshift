#pragma once
#include <fea/structure.hpp>
#include "messages.hpp"
#include "scene.hpp"
#include "interface.hpp"
#include "renderer.hpp"
#include "levelloader.hpp"
#include "levelmanager.hpp"
#include <SFML/Graphics.hpp>

class InGameState : 
    public fea::GameState,
    public QuitMessageReceiver,
    public LevelAdvanceMessageReceiver,
    public LevelRestartMessageReceiver
{
    public:
        InGameState(fea::MessageBus& bus, sf::RenderWindow& w);
        ~InGameState();
        void setup() override;
        void activate(const std::string& previous) override;
        std::string run() override;
        void handleMessage(const QuitMessage& message);
        void handleMessage(const LevelAdvanceMessage& message);
        void handleMessage(const LevelRestartMessage& message);
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

        std::string mNextState;
};
