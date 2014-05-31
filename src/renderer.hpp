#pragma once
#include "messages.hpp"
#include <SFML/Graphics.hpp>

class Renderer :
    public BGMessageReceiver,
    public ResizeMessageReceiver,
    public PlayerPositionMessageReceiver,
    public PlayerColorMessageReceiver
{
    public:
        Renderer(fea::MessageBus& b, sf::RenderWindow& w);
        ~Renderer();
        virtual void handleMessage(const BGMessage& message) override;
        virtual void handleMessage(const ResizeMessage& message) override;
        virtual void handleMessage(const PlayerPositionMessage& message) override;
        virtual void handleMessage(const PlayerColorMessage& message) override;
        void render();
    private:
        fea::MessageBus& mBus;
        sf::RenderWindow& mWindow;
        sf::Texture mBgTexture;
        sf::Sprite mBackground;
        sf::RectangleShape mPlayer;
};
