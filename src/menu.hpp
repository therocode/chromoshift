#pragma once
#include "messages.hpp"
#include <fea/structure.hpp>
#include <SFML/Graphics.hpp>

class MenuState : 
    public fea::GameState,
    public QuitMessageReceiver,
    public LevelAdvanceMessageReceiver,
    public ResizeMessageReceiver
{
    public:
        MenuState(fea::MessageBus& bus, sf::RenderWindow& w);
        ~MenuState();
        void setup() override;
        void activate(const std::string& previous) override;
        std::string run() override;

        void handleMessage(const QuitMessage& message) override;
        void handleMessage(const LevelAdvanceMessage& message) override;
        void handleMessage(const ResizeMessage& message) override;

    private:
        fea::MessageBus& mBus;
        sf::RenderWindow& mWindow;
        
        std::string mNextState;

        sf::Texture splashScreenTexture;
        sf::Sprite splashScreenSprite;
};
