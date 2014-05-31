#pragma once
#include "messages.hpp"
#include <SFML/Graphics.hpp>

class Renderer :
    public BGMessageReceiver
{
    public:
        Renderer(sf::RenderWindow& w);
        virtual void handleMessage(const BGMessage& message) override;
    private:
        sf::RenderWindow& mWindow;
        sf::Texture mBgTexture;
};
