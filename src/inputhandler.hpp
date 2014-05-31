#pragma once
#include <SFML/Window.hpp>
#include <fea/messaging.hpp>
#include <array>

class InputHandler
{
    public:
        InputHandler(fea::MessageBus& bus, sf::Window& window);
        void process();
    private:
        sf::Window& mWindow;
        fea::MessageBus& mBus;

        std::array<bool, 4> mDirectionsPressed;
};
