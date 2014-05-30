#pragma once
#include <fea/structure.hpp>
#include "messages.hpp"

class InGameState : 
    public fea::GameState,
    public QuitMessageReceiver
{
    public:
        InGameState(fea::MessageBus& b);
        ~InGameState();
        void setup() override;
        void activate(const std::string& previous) override;
        std::string run() override;
        void handleMessage(const QuitMessage& message);
    private:
        fea::MessageBus& bus;

        std::string nextState;
};
