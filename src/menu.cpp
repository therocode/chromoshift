#include "menu.hpp"
#include "texturemaker.hpp"

MenuState::MenuState(fea::MessageBus& bus, fea::Renderer2D& r) : 
    mBus(bus),
    mRenderer(r)
{
    mBus.addSubscriber<QuitMessage>(*this);
    mBus.addSubscriber<AnyKeyPressedMessage>(*this);
    mBus.addSubscriber<ResizeMessage>(*this);
}

MenuState::~MenuState()
{
    mBus.removeSubscriber<QuitMessage>(*this);
    mBus.removeSubscriber<AnyKeyPressedMessage>(*this);
    mBus.removeSubscriber<ResizeMessage>(*this);
}

void MenuState::setup()
{
    splashScreenTexture = makeTexture("textures/splashscreen.png");

    splashScreenSprite = fea::Quad({800.0f, 600.0f});
    splashScreenSprite.setTexture(splashScreenTexture);
    //splashScreenSprite.setScale({20.0f, 20.0f});
}

void MenuState::activate(const std::string& previous)
{
}

std::string MenuState::run()
{
    mRenderer.clear();
    mRenderer.queue(splashScreenSprite);
    mRenderer.render();

    return mNextState;
}

void MenuState::handleMessage(const QuitMessage& message)
{
    mNextState = "NONE";
}

void MenuState::handleMessage(const AnyKeyPressedMessage& message)
{
    mNextState = "ingame";
}

void MenuState::handleMessage(const ResizeMessage& message)
{
    //glm::uvec2 screenSize;
    //std::tie(screenSize) = message.mData;
    //sf::View newView(sf::FloatRect(0.0f, 0.0f, screenSize.x, screenSize.y));
    //mWindow.setView(newView);
}
