#include "inputhandler.hpp"
#include "messages.hpp"

InputHandler::InputHandler(fea::MessageBus& bus, sf::Window& window)    :
    mBus(bus),
    mWindow(window)
{
    mDirectionsPressed.fill(false);
}

void InputHandler::process()
{
    sf::Event event;
    while(mWindow.pollEvent(event))
    {
        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Q)
                mBus.send(QuitMessage());
            else if(event.key.code == sf::Keyboard::A)
                mDirectionsPressed[0] = true;
            else if(event.key.code == sf::Keyboard::D)
                mDirectionsPressed[1] = true;
            else if(event.key.code == sf::Keyboard::W)
                mDirectionsPressed[2] = true;
            else if(event.key.code == sf::Keyboard::S)
                mDirectionsPressed[3] = true;
        }
        else if(event.type == sf::Event::KeyReleased)
        {
            if(event.key.code == sf::Keyboard::A)
                mDirectionsPressed[0] = false;
            else if(event.key.code == sf::Keyboard::D)
                mDirectionsPressed[1] = false;
            else if(event.key.code == sf::Keyboard::W)
                mDirectionsPressed[2] = false;
            else if(event.key.code == sf::Keyboard::S)
                mDirectionsPressed[3] = false;
        }
        else if(event.type == sf::Event::Closed)
        {
            mBus.send(QuitMessage());
        }
    }

    //mBus.send(CameraDirectionMessage({(mDirectionsPressed[0] ? -10.0f : 0.0f) + (mDirectionsPressed[1] ? 10.0f : 0.0f),
    //                                 (mDirectionsPressed[2] ? -10.0f : 0.0f) + (mDirectionsPressed[3] ? 10.0f : 0.0f)}));
}
