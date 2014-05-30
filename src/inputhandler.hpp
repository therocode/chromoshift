#pragma once
#include <SFML/Window.hpp>
#include <fea/messaging.hpp>
#include <array>

class InputHandler
{
    public:
        InputHandler(fea::MessageBus& b, sf::Window& w);
        void process();
    private:
        sf::Window& window;
        fea::MessageBus& bus;

        std::array<bool, 4> directionsPressed;
};
