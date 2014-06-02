#include "menu.hpp"

MenuState::MenuState(fea::MessageBus& bus, sf::RenderWindow& w) : 
    mBus(bus),
    mWindow(w)
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
    splashScreenTexture.loadFromFile("textures/splashscreen.png");
    splashScreenSprite.setTexture(splashScreenTexture);
    splashScreenSprite.setScale({20.0f, 20.0f});
}

void MenuState::activate(const std::string& previous)
{
}

std::string MenuState::run()
{
    mWindow.clear();
    mWindow.draw(splashScreenSprite);
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
    glm::uvec2 screenSize;
    std::tie(screenSize) = message.mData;
    sf::View newView(sf::FloatRect(0.0f, 0.0f, screenSize.x, screenSize.y));
    mWindow.setView(newView);
}
