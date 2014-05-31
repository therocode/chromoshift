#pragma once
#include <fea/structure.hpp>
#include "messages.hpp"
#include "scene.hpp"
#include "renderer.hpp"
#include <SFML/Graphics.hpp>

class InGameState : 
    public fea::GameState,
    public QuitMessageReceiver
{
    public:
        InGameState(fea::MessageBus& bus, sf::RenderWindow& w);
        ~InGameState();
        void setup() override;
        void activate(const std::string& previous) override;
        std::string run() override;
        void handleMessage(const QuitMessage& message);
    private:
        fea::MessageBus& mBus;
        Scene mScene;
        Renderer mRenderer;

        std::string mNextState;
};
