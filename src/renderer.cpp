#include "renderer.hpp"

Renderer::Renderer(fea::MessageBus& b, sf::RenderWindow& w) :
    mBus(b),
    mWindow(w)
{
    mBus.addSubscriber<BGMessage>(*this);
    mBus.addSubscriber<ResizeMessage>(*this);
    mBus.addSubscriber<PlayerPositionMessage>(*this);
    mBus.addSubscriber<PlayerColorMessage>(*this);
    mBus.addSubscriber<ColourPickupCreatedMessage>(*this);

    mPlayer.setSize({30.0f, 30.0f});

    mPickupTexture.loadFromFile("textures/addsub.png");

    createPickup({5,5}, {2,5,3}, true);
}

Renderer::~Renderer()
{
    mBus.removeSubscriber<BGMessage>(*this);
    mBus.removeSubscriber<ResizeMessage>(*this);
    mBus.removeSubscriber<PlayerPositionMessage>(*this);
    mBus.removeSubscriber<PlayerColorMessage>(*this);
    mBus.removeSubscriber<ColourPickupCreatedMessage>(*this);
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
    
    sf::View view = mWindow.getView();
    view.setCenter(x * 30.0f, y * 30.0f);
    mWindow.setView(view);
}

void Renderer::handleMessage(const PlayerColorMessage& message)
{
    const glm::uvec3& color = std::get<0>(message.mData);

    mPlayer.setFillColor(glmToSFColor(color));
}

void Renderer::handleMessage(const ColourPickupCreatedMessage& message)
{
    size_t id;
    glm::uvec2 position;
    glm::uvec3 colour;
    bool additive;

    std::tie(id, position, colour, additive) = message.mData;

    mPickups.emplace(id, createPickup(position, colour, additive);
}

void Renderer::render()
{
    mWindow.clear();
    mWindow.draw(mBackground);

    for(const auto& pickup : mPickups)
    {
        mWindow.draw(pickup.second.rectangle);
        mWindow.draw(pickup.second.overlay);
    }

    mWindow.draw(mPlayer);
}

pickup Renderer::createPickup(const glm::uvec2& position, const glm::uvec3& color, bool additive)
{
    Pickup pickup;

    pickup.rectangle.setPosition({position.x * 30.0f, position.y * 30.0f});
    pickup.rectangle.setSize({30.0f, 30.0f});
    pickup.rectangle.setFillColor(glmToSFColor(color));
    pickup.overlay.setPosition({position.x * 30.0f, position.y * 30.0f});
    pickup.overlay.setTexture(mPickupTexture);
    pickup.overlay.setTextureRect({additive ? 0 : 6, 0, 6, 6});
    pickup.overlay.setScale({5.0f, 5.0f});

    return pickup;
}

sf::Color Renderer::glmToSFColor(const glm::uvec3& col) const
{
    sf::Color realColor;

    realColor.r = std::max(0, (int32_t)col.r * 64 - 1);
    realColor.g = std::max(0, (int32_t)col.g * 64 - 1);
    realColor.b = std::max(0, (int32_t)col.b * 64 - 1);

    return realColor;
}
