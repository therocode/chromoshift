#pragma once
#include "messages.hpp"

class Scene
{
    public:
        Scene(fea::MessageBus& bus);

    private:
        fea::MessageBus& mBus;
};
