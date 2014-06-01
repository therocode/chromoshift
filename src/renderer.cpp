#include "renderer.hpp"
#include <iostream>

Pickup::Pickup() :
    counter(0)
{
}

void Pickup::tick()
{
    counter++;
    if(counter == 64)
        counter = 0;

    int32_t opacity = (abs(counter / 2 - 16) / 2) + 4;

    sf::Color current = overlay.getColor();
    current.a = std::min(255, opacity * 16);
    overlay.setColor(current);
}

Renderer::Renderer(fea::MessageBus& b, sf::RenderWindow& w) :
    mBus(b),
    mWindow(w),
    tileSize({30.0f, 30.0f})
{
    mBus.addSubscriber<BGMessage>(*this);
    mBus.addSubscriber<ResizeMessage>(*this);
    mBus.addSubscriber<PlayerPositionMessage>(*this);
    mBus.addSubscriber<GoalColourMessage>(*this);
    mBus.addSubscriber<PlayerColourMessage>(*this);
    mBus.addSubscriber<ColourPickupCreatedMessage>(*this);
    mBus.addSubscriber<ColourPickupRemovedMessage>(*this);

    mPlayer.setSize({tileSize.x, tileSize.y});

    mPickupTexture.loadFromFile("textures/addsub.png");
}

Renderer::~Renderer()
{
    mBus.removeSubscriber<BGMessage>(*this);
    mBus.removeSubscriber<ResizeMessage>(*this);
    mBus.removeSubscriber<PlayerPositionMessage>(*this);
    mBus.removeSubscriber<GoalColourMessage>(*this);
    mBus.removeSubscriber<PlayerColourMessage>(*this);
    mBus.removeSubscriber<ColourPickupCreatedMessage>(*this);
    mBus.removeSubscriber<ColourPickupRemovedMessage>(*this);
}

void Renderer::handleMessage(const BGMessage& message)
{
    const sf::Image& image = std::get<0>(message.mData);

    mBgTexture.loadFromImage(image);
    mBackground.setTexture(mBgTexture);
    mBackground.setScale(tileSize.x, tileSize.y);
}

void Renderer::handleMessage(const ResizeMessage& message)
{
    glm::uvec2 screenSize;
    std::tie(screenSize) = message.mData;
    sf::View view = mWindow.getView();
    view.setSize(screenSize.x, screenSize.y);
    mWindow.setView(view);
}

void Renderer::handleMessage(const PlayerPositionMessage& message)
{
    glm::uvec2 position;

    std::tie(position) = message.mData;

    mPlayer.setPosition({position.x * tileSize.x, position.y * tileSize.y});
    
    sf::View view = mWindow.getView();
    view.setCenter(position.x * tileSize.x, position.y * tileSize.y);
    mWindow.setView(view);
}

void Renderer::handleMessage(const GoalColourMessage& message)
{
    mGoalColour = std::get<0>(message.mData);
}

void Renderer::handleMessage(const PlayerColourMessage& message)
{
    mPlayerColour = std::get<0>(message.mData);

    mPlayer.setFillColor(glmToSFColour(mPlayerColour));

    mPlayerColourMeter.clear();
    for(uint32_t i = 0; i < mGoalColour.r; i++)
    {
        //sf::RectangleShape
    /*
    pickup.rectangle.setPosition({position.x * tileSize.x, position.y * tileSize.y});
    pickup.rectangle.setSize({tileSize.x, tileSize.y});
    pickup.rectangle.setFillColor(glmToSFColour(color));
    */
    }
}

void Renderer::handleMessage(const ColourPickupCreatedMessage& message)
{
    size_t id;
    glm::uvec2 position;
    glm::uvec3 colour;
    bool additive;

    std::tie(id, position, colour, additive) = message.mData;

    mPickups.emplace(id, createPickup(position, colour, additive));
}

void Renderer::handleMessage(const ColourPickupRemovedMessage& message)
{
    size_t id;

    std::tie(id) = message.mData;

    mPickups.erase(id);
}

void Renderer::render()
{
    mWindow.clear();
    mWindow.draw(mBackground);

    for(auto& pickup : mPickups)
    {
        mWindow.draw(pickup.second.rectangle);
        mWindow.draw(pickup.second.overlay);
        pickup.second.tick();
    }

    mWindow.draw(mPlayer);

    /*
    mWindow.draw(mInterfaceBackground);
    mWindow.draw(mGoalColours);
    mWindow.draw(mPlayerColours);
    */
}

Pickup Renderer::createPickup(const glm::uvec2& position, const glm::uvec3& color, bool additive)
{
    Pickup pickup;

    pickup.rectangle.setPosition({position.x * tileSize.x, position.y * tileSize.y});
    pickup.rectangle.setSize({tileSize.x, tileSize.y});
    pickup.rectangle.setFillColor(glmToSFColour(color));
    pickup.overlay.setPosition({position.x * tileSize.x, position.y * tileSize.y});
    pickup.overlay.setTexture(mPickupTexture);
    pickup.overlay.setTextureRect({additive ? 0 : 6, 0, 6, 6});
    pickup.overlay.setScale({5.0f, 5.0f});

    return pickup;
}

sf::Color Renderer::glmToSFColour(const glm::uvec3& col) const
{
    sf::Color realColor;

    realColor.r = std::max(0, (int32_t)col.r * 64 - 1);
    realColor.g = std::max(0, (int32_t)col.g * 64 - 1);
    realColor.b = std::max(0, (int32_t)col.b * 64 - 1);

    return realColor;
}
