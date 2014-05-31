#include "renderer.hpp"

Renderer::Renderer(fea::MessageBus& b, sf::RenderWindow& w) :
    mBus(b),
    mWindow(w)
{
    mBus.addSubscriber<BGMessage>(*this);
    mBus.addSubscriber<ResizeMessage>(*this);
    mBus.addSubscriber<PlayerPositionMessage>(*this);
    mBus.addSubscriber<PlayerColorMessage>(*this);

    mPlayer.setSize({30.0f, 30.0f});
}

Renderer::~Renderer()
{
    mBus.removeSubscriber<BGMessage>(*this);
    mBus.removeSubscriber<ResizeMessage>(*this);
    mBus.removeSubscriber<PlayerPositionMessage>(*this);
    mBus.removeSubscriber<PlayerColorMessage>(*this);
}

void Renderer::handleMessage(const BGMessage& message)
{
    const sf::Image& image = std::get<0>(message.mData);

    mBgTexture.loadFromImage(image);
    mBackground.setTexture(mBgTexture);
    mBackground.setScale(30.0f, 30.0f);
}

void Renderer::handleMessage(const ResizeMessage& message)
{
    uint32_t width;
    uint32_t height;

    std::tie(width, height) = message.mData;
    sf::View view = mWindow.getView();
    view.setSize(width, height);
    mWindow.setView(view);
}

void Renderer::handleMessage(const PlayerPositionMessage& message)
{
    uint32_t x;
    uint32_t y;

    std::tie(x, y) = message.mData;

    mPlayer.setPosition({x * 30.0f, y * 30.0f});
}

void Renderer::handleMessage(const PlayerColorMessage& message)
{
    const sf::Color& color = std::get<0>(message.mData);

    mPlayer.setFillColor(color);
}

void Renderer::render()
{
    mWindow.clear();
    mWindow.draw(mBackground);
    mWindow.draw(mPlayer);
}
