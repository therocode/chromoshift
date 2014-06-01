#pragma once
#include "messages.h"

class Interface
{
    public:
        Interface(const fea::MessageBus& bus);
        ~Interface();

    private:
        fea::MessageBus& mBus;

        glm::uvec3 mPlayerColour;
        glm::uvec3 mGoalColour;
};
