#pragma once
#include "messages.hpp"

class Interface :
    public fea::MessageReceiver<PlayerColourMessage>,
    public fea::MessageReceiver<GoalColourMessage>
{
    public:
        Interface(fea::MessageBus& bus);
        ~Interface();

        virtual void handleMessage(const PlayerColourMessage& message) override;
        virtual void handleMessage(const GoalColourMessage& message) override;

    private:
        fea::MessageBus& mBus;

        glm::uvec3 mPlayerColour;
        glm::uvec3 mGoalColour;
};
