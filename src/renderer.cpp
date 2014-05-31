#include "renderer.hpp"

Renderer::Renderer(fea::MessageBus& b, sf::RenderWindow& w) :
    mBus(b),
    mWindow(w)
{
    mBus.addSubscriber<BGMessage>(*this);
}

Renderer::~Renderer()
{
    mBus.removeSubscriber<BGMessage>(*this);
}

void Renderer::handleMessage(const BGMessage& message)
{
    const sf::Image& image = std::get<0>(message.mData);

    mBgTexture.loadFromImage(image);
    mBackground.setTexture(mBgTexture);
}

void Renderer::render()
{
    mWindow.clear();
    mWindow.draw(mBackground);
}
