#include "renderer.hpp"

Renderer::Renderer(fea::MessageBus& b, sf::RenderWindow& w) :
    mBus(b),
    mWindow(w)
{
    mBus.addSubscriber<BGMessage>(*this);
    mBus.addSubscriber<ResizeMessage>(*this);
}

Renderer::~Renderer()
{
    mBus.removeSubscriber<BGMessage>(*this);
    mBus.removeSubscriber<ResizeMessage>(*this);
}

void Renderer::handleMessage(const BGMessage& message)
{
    const sf::Image& image = std::get<0>(message.mData);

    mBgTexture.loadFromImage(image);
    mBackground.setTexture(mBgTexture);
    mBackground.setScale(40.0f, 40.0f);
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

void Renderer::render()
{
    mWindow.clear();
    mWindow.draw(mBackground);
}
