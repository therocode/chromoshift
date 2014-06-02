#include "inputhandler.hpp"
#include "messages.hpp"

InputHandler::InputHandler(fea::MessageBus& bus, fea::InputHandler& handler)    :
    mBus(bus),
    mHandler(handler)
{
    mDirectionsPressed.fill(false);
}

void InputHandler::process()
{
    fea::Event event;
    while(mHandler.pollEvent(event))
    {
        if(event.type == fea::Event::KEYPRESSED)
        {
            mBus.send(AnyKeyPressedMessage());
            if(event.key.code == fea::Keyboard::Q)
                mBus.send(QuitMessage());
            else if(event.key.code == fea::Keyboard::A)
                mBus.send(MoveMessage(LEFT));
                //mDirectionsPressed[0] = true;
            else if(event.key.code == fea::Keyboard::D)
                mBus.send(MoveMessage(RIGHT));
                //mDirectionsPressed[1] = true;
            else if(event.key.code == fea::Keyboard::W)
                mBus.send(MoveMessage(UP));
                //mDirectionsPressed[2] = true;
            else if(event.key.code == fea::Keyboard::S)
                mBus.send(MoveMessage(DOWN));
                //mDirectionsPressed[3] = true;
            else if(event.key.code == fea::Keyboard::N)
                mBus.send(LevelAdvanceMessage(1));
            else if(event.key.code == fea::Keyboard::P)
                mBus.send(LevelAdvanceMessage(-1));
            else if(event.key.code == fea::Keyboard::R)
                mBus.send(LevelRestartMessage());
        }
        else if(event.type == fea::Event::KEYRELEASED)
        {
            if(event.key.code == fea::Keyboard::A)
                mDirectionsPressed[0] = false;
            else if(event.key.code == fea::Keyboard::D)
                mDirectionsPressed[1] = false;
            else if(event.key.code == fea::Keyboard::W)
                mDirectionsPressed[2] = false;
            else if(event.key.code == fea::Keyboard::S)
                mDirectionsPressed[3] = false;
        }
        else if(event.type == fea::Event::CLOSED)
        {
            mBus.send(QuitMessage());
        }
        else if(event.type == fea::Event::RESIZED)
        {
            mBus.send(ResizeMessage({event.size.width, event.size.height}));
        }
    }

    //mBus.send(CameraDirectionMessage({(mDirectionsPressed[0] ? -10.0f : 0.0f) + (mDirectionsPressed[1] ? 10.0f : 0.0f),
    //                                 (mDirectionsPressed[2] ? -10.0f : 0.0f) + (mDirectionsPressed[3] ? 10.0f : 0.0f)}));
}
