#include "renderer.hpp"
#include <iostream>

const sf::Color goalRed(255, 120, 120);
const sf::Color goalGreen(120, 255, 120);
const sf::Color goalBlue(120, 120, 255);
const sf::Color playerRed(255, 0, 0);
const sf::Color playerGreen(0, 255, 0);
const sf::Color playerBlue(0, 0, 255);

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
    mTileSize({30.0f, 30.0f}),
    mInterfacePosition({0.0f, 500.0f}),
    mAnimationTimer(0)
{
    mBus.addSubscriber<BGMessage>(*this);
    mBus.addSubscriber<ResizeMessage>(*this);
    mBus.addSubscriber<PlayerPositionMessage>(*this);
    mBus.addSubscriber<GoalColourMessage>(*this);
    mBus.addSubscriber<PlayerColourMessage>(*this);
    mBus.addSubscriber<ColourPickupCreatedMessage>(*this);
    mBus.addSubscriber<ColourPickupRemovedMessage>(*this);
    mBus.addSubscriber<BackgroundColourMessage>(*this);

    mPlayer.setSize({mTileSize.x, mTileSize.y});

    mPickupTexture.loadFromFile("textures/addsub.png");

    mInterfaceTexture.loadFromFile("textures/rgb.png");
    mInterfaceSprite.setTexture(mInterfaceTexture);
    mInterfaceSprite.setScale({5.0f, 5.0f});
    mInterfaceSprite.setPosition({(float)mInterfacePosition.x, (float)mInterfacePosition.y});

    mInterfaceOverlayTexture.loadFromFile("textures/rgb-overlay.png");
    mInterfaceOverlaySprite.setTexture(mInterfaceOverlayTexture);
    mInterfaceOverlaySprite.setScale({5.0f, 5.0f});
    mInterfaceOverlaySprite.setPosition({(float)mInterfacePosition.x, (float)mInterfacePosition.y});

    mSceneView.reset({0, 0, 800, 600});
    mInterfaceView.reset({0, 0, 800, 600});
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
    mBus.removeSubscriber<BackgroundColourMessage>(*this);
}

void Renderer::handleMessage(const BGMessage& message)
{
    const sf::Image& image = std::get<0>(message.mData);

    mBgTexture.loadFromImage(image);
    mBackground = sf::Sprite();
    mBackground.setTexture(mBgTexture);
    mBackground.setScale(mTileSize.x, mTileSize.y);
    mAnimationInfo = glm::ivec3();
}

void Renderer::handleMessage(const ResizeMessage& message)
{
    glm::uvec2 screenSize;
    std::tie(screenSize) = message.mData;
    mSceneView.setSize(screenSize.x, screenSize.y);
    mInterfaceView.setSize(screenSize.x, screenSize.y);
}

void Renderer::handleMessage(const PlayerPositionMessage& message)
{
    glm::uvec2 position;

    std::tie(position) = message.mData;

    mPlayer.setPosition({position.x * mTileSize.x, position.y * mTileSize.y});
    
    mSceneView.setCenter(position.x * mTileSize.x, position.y * mTileSize.y);
}

void Renderer::handleMessage(const GoalColourMessage& message)
{
    mGoalColour = std::get<0>(message.mData);

    mGoalColourMeter.clear();
    mInterfaceOverlaySprite.setColor(glmToSFColour(mGoalColour));
    for(uint32_t i = 0; i < mGoalColour.r; i++)   //r
    {
        sf::RectangleShape rect;
        rect.setPosition({mInterfacePosition.x + (mTileSize.x * (i + 2)), mInterfacePosition.y + (mTileSize.y * 0)});
        rect.setSize({mTileSize.x, mTileSize.y});
        rect.setFillColor(goalRed);
        mGoalColourMeter[RED].push_back(rect);
    }
    for(uint32_t i = 0; i < mGoalColour.g; i++)   //g
    {
        sf::RectangleShape rect;
        rect.setPosition({mInterfacePosition.x + (mTileSize.x * (i + 2)), mInterfacePosition.y + (mTileSize.y * 1)});
        rect.setSize({mTileSize.x, mTileSize.y});
        rect.setFillColor(goalGreen);
        mGoalColourMeter[GREEN].push_back(rect);
    }
    for(uint32_t i = 0; i < mGoalColour.b; i++)   //b
    {
        sf::RectangleShape rect;
        rect.setPosition({mInterfacePosition.x + (mTileSize.x * (i + 2)), mInterfacePosition.y + (mTileSize.y * 2)});
        rect.setSize({mTileSize.x, mTileSize.y});
        rect.setFillColor(goalBlue);
        mGoalColourMeter[BLUE].push_back(rect);
    }
}

void Renderer::handleMessage(const PlayerColourMessage& message)
{
    mPlayerColour = std::get<0>(message.mData);

    mPlayer.setFillColor(glmToSFColour(mPlayerColour));

    mPlayerColourMeter.clear();
    for(uint32_t i = 0; i < mPlayerColour.r; i++)   //r
    {
        sf::RectangleShape rect;
        rect.setPosition({mInterfacePosition.x + (mTileSize.x * (i + 2)), mInterfacePosition.y + (mTileSize.y * 0)});
        rect.setSize({mTileSize.x, mTileSize.y});
        rect.setFillColor(playerRed);
        mPlayerColourMeter[RED].push_back(rect);
    }
    for(uint32_t i = 0; i < mPlayerColour.g; i++)   //g
    {
        sf::RectangleShape rect;
        rect.setPosition({mInterfacePosition.x + (mTileSize.x * (i + 2)), mInterfacePosition.y + (mTileSize.y * 1)});
        rect.setSize({mTileSize.x, mTileSize.y});
        rect.setFillColor(playerGreen);
        mPlayerColourMeter[GREEN].push_back(rect);
    }
    for(uint32_t i = 0; i < mPlayerColour.b; i++)   //b
    {
        sf::RectangleShape rect;
        rect.setPosition({mInterfacePosition.x + (mTileSize.x * (i + 2)), mInterfacePosition.y + (mTileSize.y * 2)});
        rect.setSize({mTileSize.x, mTileSize.y});
        rect.setFillColor(playerBlue);
        mPlayerColourMeter[BLUE].push_back(rect);
    }


    mOverlayMeter.clear();
    for(uint32_t i = 0; i < 4; i++)
    {
        sf::RectangleShape rect;
        rect.setPosition({mInterfacePosition.x + (mTileSize.x * (i + 2)), mInterfacePosition.y + (mTileSize.y * 0)});
        rect.setSize({mTileSize.x, mTileSize.y});
        rect.setFillColor(sf::Color::Black);
        mOverlayMeter[RED].push_back(rect);
    }
    for(uint32_t i = 0; i < 4; i++)
    {
        sf::RectangleShape rect;
        rect.setPosition({mInterfacePosition.x + (mTileSize.x * (i + 2)), mInterfacePosition.y + (mTileSize.y * 1)});
        rect.setSize({mTileSize.x, mTileSize.y});
        rect.setFillColor(sf::Color::Black);
        mOverlayMeter[GREEN].push_back(rect);
    }
    for(uint32_t i = 0; i < 4; i++)
    {
        sf::RectangleShape rect;
        rect.setPosition({mInterfacePosition.x + (mTileSize.x * (i + 2)), mInterfacePosition.y + (mTileSize.y * 2)});
        rect.setSize({mTileSize.x, mTileSize.y});
        rect.setFillColor(sf::Color::Black);
        mOverlayMeter[BLUE].push_back(rect);
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

void Renderer::handleMessage(const BackgroundColourMessage& message)
{
    mBackgroundColor = std::get<0>(message.mData);
}

void Renderer::handleMessage(const PlayerDiedMessage& message)
{
    mAnimationInfo = std::get<0>(message.mData);
}

void Renderer::render()
{
    mWindow.clear(mBackgroundColor);

    // scene //
    mWindow.setView(mSceneView);

    mWindow.draw(mBackground);
    for(auto& pickup : mPickups)
    {
        mWindow.draw(pickup.second.rectangle);
        mWindow.draw(pickup.second.overlay);
        pickup.second.tick();
    }
    mWindow.draw(mPlayer);

    // interface //
    mWindow.setView(mInterfaceView);

    mWindow.draw(mInterfaceSprite);
    mWindow.draw(mInterfaceOverlaySprite);
    for(auto& map : mGoalColourMeter)
    {
        for(auto& rect : map.second)
            mWindow.draw(rect);
    }
    for(auto& map : mPlayerColourMeter)
    {
        for(auto& rect : map.second)
            mWindow.draw(rect);
    }

    if(mAnimationInfo.r == -1)
        for(auto& player : mOverlayMeter[RED])
        {
            mWindow.draw(player);
        }
    if(mAnimationInfo.g == -1)
        for(auto& player : mOverlayMeter[GREEN])
        {
            mWindow.draw(player);
        }
    if(mAnimationInfo.b == -1)
        for(auto& player : mOverlayMeter[BLUE])
        {
            mWindow.draw(player);
        }

    updateInterface();
}

Pickup Renderer::createPickup(const glm::uvec2& position, const glm::uvec3& color, bool additive)
{
    Pickup pickup;

    pickup.rectangle.setPosition({position.x * mTileSize.x, position.y * mTileSize.y});
    pickup.rectangle.setSize({mTileSize.x, mTileSize.y});
    pickup.rectangle.setFillColor(glmToSFColour(color));
    pickup.overlay.setPosition({position.x * mTileSize.x, position.y * mTileSize.y});
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

void Renderer::updateInterface()
{
    mAnimationTimer++;

    if(mAnimationTimer == 46)
        mAnimationTimer = 0;

    if(mAnimationTimer == 23)
    {
        if(mAnimationInfo.r == 1)
            for(auto& player : mPlayerColourMeter[RED])
            {
                player.setFillColor(playerRed);
            }
        if(mAnimationInfo.g == 1)
            for(auto& player : mPlayerColourMeter[GREEN])
            {
                player.setFillColor(playerGreen);
            }
        if(mAnimationInfo.b == 1)
            for(auto& player : mPlayerColourMeter[BLUE])
            {
                player.setFillColor(playerBlue);
            }

        if(mAnimationInfo.r == -1)
            for(auto& player : mOverlayMeter[RED])
            {
                player.setFillColor(playerRed - sf::Color(200, 0, 0, 0));
            }
        if(mAnimationInfo.g == -1)
            for(auto& player : mOverlayMeter[GREEN])
            {
                player.setFillColor(playerGreen - sf::Color(0, 200, 0, 0));
            }
        if(mAnimationInfo.b == -1)
            for(auto& player : mOverlayMeter[BLUE])
            {
                player.setFillColor(playerBlue - sf::Color(0, 0, 200, 0));
            }
    }

    if(mAnimationTimer == 45)
    {
        if(mAnimationInfo.r == 1)
            for(auto& player : mPlayerColourMeter[RED])
            {
                player.setFillColor(playerRed + sf::Color(0, 100, 100));
            }
        if(mAnimationInfo.g == 1)
            for(auto& player : mPlayerColourMeter[GREEN])
            {
                player.setFillColor(playerGreen + sf::Color(100, 0, 100));
            }
        if(mAnimationInfo.b == 1)
            for(auto& player : mPlayerColourMeter[BLUE])
            {
                player.setFillColor(playerBlue + sf::Color(100, 100, 0));
            }

        if(mAnimationInfo.r == -1)
            for(auto& player : mOverlayMeter[RED])
            {
                player.setFillColor(playerRed - sf::Color(230, 0, 0, 0));
            }
        if(mAnimationInfo.g == -1)
            for(auto& player : mOverlayMeter[GREEN])
            {
                player.setFillColor(playerGreen - sf::Color(0, 230, 0, 0));
            }
        if(mAnimationInfo.b == -1)
            for(auto& player : mOverlayMeter[BLUE])
            {
                player.setFillColor(playerBlue - sf::Color(0, 0, 230, 0));
            }
    }
}
