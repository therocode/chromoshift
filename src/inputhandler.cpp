#include "inputhandler.hpp"
#include "messages.hpp"

InputHandler::InputHandler(fea::MessageBus& b, sf::Window& w)    :
    bus(b),
    window(w)
{
    directionsPressed.fill(false);
}

void InputHandler::process()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Q)
                bus.send(QuitMessage());
            else if(event.key.code == sf::Keyboard::A)
                directionsPressed[0] = true;
            else if(event.key.code == sf::Keyboard::D)
                directionsPressed[1] = true;
            else if(event.key.code == sf::Keyboard::W)
                directionsPressed[2] = true;
            else if(event.key.code == sf::Keyboard::S)
                directionsPressed[3] = true;
        }
        else if(event.type == sf::Event::KeyReleased)
        {
            if(event.key.code == sf::Keyboard::A)
                directionsPressed[0] = false;
            else if(event.key.code == sf::Keyboard::D)
                directionsPressed[1] = false;
            else if(event.key.code == sf::Keyboard::W)
                directionsPressed[2] = false;
            else if(event.key.code == sf::Keyboard::S)
                directionsPressed[3] = false;
        }
        else if(event.type == sf::Event::Closed)
        {
            bus.send(QuitMessage());
        }
    }

    //bus.send(CameraDirectionMessage({(directionsPressed[0] ? -10.0f : 0.0f) + (directionsPressed[1] ? 10.0f : 0.0f),
    //                                 (directionsPressed[2] ? -10.0f : 0.0f) + (directionsPressed[3] ? 10.0f : 0.0f)}));
}
