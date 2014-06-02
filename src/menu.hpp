#pragma once
#include "messages.hpp"
#include <fea/structure.hpp>
#include <fea/render2d.hpp>

class MenuState : 
    public fea::GameState,
    public QuitMessageReceiver,
    public AnyKeyPressedMessageReceiver,
    public ResizeMessageReceiver
{
    public:
        MenuState(fea::MessageBus& bus, fea::Renderer2D& r);
        ~MenuState();
        void setup() override;
        void activate(const std::string& previous) override;
        std::string run() override;

        void handleMessage(const QuitMessage& message) override;
        void handleMessage(const AnyKeyPressedMessage& message) override;
        void handleMessage(const ResizeMessage& message) override;

    private:
        fea::MessageBus& mBus;
        fea::Renderer2D& mRenderer;
        
        std::string mNextState;

        fea::Texture splashScreenTexture;
        fea::Quad splashScreenSprite;
};
