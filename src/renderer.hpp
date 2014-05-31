#pragma once
#include "messages.hpp"
#include <SFML/Graphics.hpp>

class Renderer :
    public BGMessageReceiver
{
    public:
        Renderer(fea::MessageBus& b, sf::RenderWindow& w);
        ~Renderer();
        virtual void handleMessage(const BGMessage& message) override;
        void render();
    private:
        fea::MessageBus& mBus;
        sf::RenderWindow& mWindow;
        sf::Texture mBgTexture;
        sf::Sprite mBackground;
};
