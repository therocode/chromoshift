#pragma once
#include <fea/messaging.hpp>
#include <fea/userinterface.hpp>
#include <array>

class InputHandler
{
    public:
        InputHandler(fea::MessageBus& bus, fea::InputHandler& handler);
        void process();
    private:
        fea::InputHandler& mHandler;
        fea::MessageBus& mBus;

        std::array<bool, 4> mDirectionsPressed;
};
