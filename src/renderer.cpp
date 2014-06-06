#include "renderer.hpp"
#include <iostream>
#include "texturemaker.hpp"

const fea::Color goalRed(255, 120, 120);
const fea::Color goalGreen(120, 255, 120);
const fea::Color goalBlue(120, 120, 255);
const fea::Color playerRed(255, 0, 0);
const fea::Color playerGreen(0, 255, 0);
const fea::Color playerBlue(0, 0, 255);

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

    fea::Color current = overlay.getColor();
    overlay.setOpacity((float)opacity / 16.0f);
}

Renderer::Renderer(fea::MessageBus& b, fea::Renderer2D& r) :
    mBus(b),
    mRenderer(r),
    mTileSize({30.0f, 30.0f}),
    mInterfacePosition({-400.0f, 200.0f}),
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
    mBus.addSubscriber<PlayerDiedMessage>(*this);

    mPlayer.setSize({mTileSize.x, mTileSize.y});

    mPickupTexture = makeTexture("textures/addsub.png");

    mInterfaceTexture = makeTexture("textures/rgb.png");
    mInterfaceSprite = fea::Quad((glm::vec2)mInterfaceTexture.getSize());
    mInterfaceSprite.setTexture(mInterfaceTexture);
    mInterfaceSprite.setScale({5.0f, 5.0f});
    mInterfaceSprite.setPosition({mInterfacePosition.x, mInterfacePosition.y});
    mInterfaceSprite.setParallax({0.0f, 0.0f});

    mInterfaceOverlayTexture = makeTexture("textures/rgb-overlay.png");
    mInterfaceOverlaySprite = fea::Quad((glm::vec2)mInterfaceOverlayTexture.getSize());
    mInterfaceOverlaySprite.setTexture(mInterfaceOverlayTexture);
    mInterfaceOverlaySprite.setScale({5.0f, 5.0f});
    mInterfaceOverlaySprite.setPosition({mInterfacePosition.x, mInterfacePosition.y});
    mInterfaceOverlaySprite.setParallax({0.0f, 0.0f});
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
    mBus.removeSubscriber<PlayerDiedMessage>(*this);
}

void Renderer::handleMessage(const BGMessage& message)
{
    fea::Texture& image = std::get<0>(message.mData);

    mBgTexture = std::move(image);
    mBackground = fea::Quad((glm::vec2)mBgTexture.getSize());
    mBackground.setTexture(mBgTexture);
    mBackground.setScale({mTileSize.x, mTileSize.y});
    mAnimationInfo = glm::ivec3();
}

void Renderer::handleMessage(const ResizeMessage& message)
{
    glm::uvec2 screenSize;
    std::tie(screenSize) = message.mData;
}

void Renderer::handleMessage(const PlayerPositionMessage& message)
{
    glm::uvec2 position;

    std::tie(position) = message.mData;

    mPlayer.setPosition({position.x * mTileSize.x, position.y * mTileSize.y});
    
    mRenderer.getViewport().getCamera().setPosition({position.x * mTileSize.x, position.y * mTileSize.y});
}

void Renderer::handleMessage(const GoalColourMessage& message)
{
    mGoalColour = std::get<0>(message.mData);

    mGoalColourMeter.clear();
    mInterfaceOverlaySprite.setColor(glmToFeaColour(mGoalColour));
    for(uint32_t i = 0; i < mGoalColour.r; i++)   //r
    {
        fea::Quad rect;
        rect.setPosition({mInterfacePosition.x + (mTileSize.x * (i + 2)), mInterfacePosition.y + (mTileSize.y * 0)});
        rect.setSize({mTileSize.x, mTileSize.y});
        rect.setColor(goalRed);
        rect.setParallax({0.0f, 0.0f});
        mGoalColourMeter[RED].push_back(rect);
    }
    for(uint32_t i = 0; i < mGoalColour.g; i++)   //g
    {
        fea::Quad rect;
        rect.setPosition({mInterfacePosition.x + (mTileSize.x * (i + 2)), mInterfacePosition.y + (mTileSize.y * 1)});
        rect.setSize({mTileSize.x, mTileSize.y});
        rect.setColor(goalGreen);
        rect.setParallax({0.0f, 0.0f});
        mGoalColourMeter[GREEN].push_back(rect);
    }
    for(uint32_t i = 0; i < mGoalColour.b; i++)   //b
    {
        fea::Quad rect;
        rect.setPosition({mInterfacePosition.x + (mTileSize.x * (i + 2)), mInterfacePosition.y + (mTileSize.y * 2)});
        rect.setSize({mTileSize.x, mTileSize.y});
        rect.setColor(goalBlue);
        rect.setParallax({0.0f, 0.0f});
        mGoalColourMeter[BLUE].push_back(rect);
    }
}

void Renderer::handleMessage(const PlayerColourMessage& message)
{
    mPlayerColour = std::get<0>(message.mData);

    mPlayer.setColor(glmToFeaColour(mPlayerColour));

    mPlayerColourMeter.clear();
    for(uint32_t i = 0; i < mPlayerColour.r; i++)   //r
    {
        fea::Quad rect;
        rect.setPosition({mInterfacePosition.x + (mTileSize.x * (i + 2)), mInterfacePosition.y + (mTileSize.y * 0)});
        rect.setSize({mTileSize.x, mTileSize.y});
        rect.setColor(playerRed);
        rect.setParallax({0.0f, 0.0f});
        mPlayerColourMeter[RED].push_back(rect);
    }
    for(uint32_t i = 0; i < mPlayerColour.g; i++)   //g
    {
        fea::Quad rect;
        rect.setPosition({mInterfacePosition.x + (mTileSize.x * (i + 2)), mInterfacePosition.y + (mTileSize.y * 1)});
        rect.setSize({mTileSize.x, mTileSize.y});
        rect.setColor(playerGreen);
        rect.setParallax({0.0f, 0.0f});
        mPlayerColourMeter[GREEN].push_back(rect);
    }
    for(uint32_t i = 0; i < mPlayerColour.b; i++)   //b
    {
        fea::Quad rect;
        rect.setPosition({mInterfacePosition.x + (mTileSize.x * (i + 2)), mInterfacePosition.y + (mTileSize.y * 2)});
        rect.setSize({mTileSize.x, mTileSize.y});
        rect.setColor(playerBlue);
        rect.setParallax({0.0f, 0.0f});
        mPlayerColourMeter[BLUE].push_back(rect);
    }


    mOverlayMeter.clear();
    for(uint32_t i = 0; i < 4; i++)
    {
        fea::Quad rect;
        rect.setPosition({mInterfacePosition.x + (mTileSize.x * (i + 2)), mInterfacePosition.y + (mTileSize.y * 0)});
        rect.setSize({mTileSize.x, mTileSize.y});
        rect.setColor(fea::Color::Black);
        rect.setParallax({0.0f, 0.0f});
        mOverlayMeter[RED].push_back(rect);
    }
    for(uint32_t i = 0; i < 4; i++)
    {
        fea::Quad rect;
        rect.setPosition({mInterfacePosition.x + (mTileSize.x * (i + 2)), mInterfacePosition.y + (mTileSize.y * 1)});
        rect.setSize({mTileSize.x, mTileSize.y});
        rect.setColor(fea::Color::Black);
        rect.setParallax({0.0f, 0.0f});
        mOverlayMeter[GREEN].push_back(rect);
    }
    for(uint32_t i = 0; i < 4; i++)
    {
        fea::Quad rect;
        rect.setPosition({mInterfacePosition.x + (mTileSize.x * (i + 2)), mInterfacePosition.y + (mTileSize.y * 2)});
        rect.setSize({mTileSize.x, mTileSize.y});
        rect.setColor(fea::Color::Black);
        rect.setParallax({0.0f, 0.0f});
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
    mRenderer.clear(mBackgroundColor);

    // scene //
    mRenderer.queue(mBackground);
    for(auto& pickup : mPickups)
    {
        mRenderer.queue(pickup.second.rectangle);
        mRenderer.queue(pickup.second.overlay);
        pickup.second.tick();
    }
    mRenderer.queue(mPlayer);

    // interface //
    mRenderer.queue(mInterfaceSprite);
    mRenderer.queue(mInterfaceOverlaySprite);
    for(auto& map : mGoalColourMeter)
    {
        for(auto& rect : map.second)
            mRenderer.queue(rect);
    }
    for(auto& map : mPlayerColourMeter)
    {
        for(auto& rect : map.second)
            mRenderer.queue(rect);
    }

    if(mAnimationInfo.r == -1)
        for(auto& player : mOverlayMeter[RED])
        {
            mRenderer.queue(player);
        }
    if(mAnimationInfo.g == -1)
        for(auto& player : mOverlayMeter[GREEN])
        {
            mRenderer.queue(player);
        }
    if(mAnimationInfo.b == -1)
        for(auto& player : mOverlayMeter[BLUE])
        {
            mRenderer.queue(player);
        }

    mRenderer.render();

    updateInterface();
}

Pickup Renderer::createPickup(const glm::uvec2& position, const glm::uvec3& color, bool additive)
{
    Pickup pickup;

    pickup.rectangle.setPosition({position.x * mTileSize.x, position.y * mTileSize.y});
    pickup.rectangle.setSize({mTileSize.x, mTileSize.y});
    pickup.rectangle.setColor(glmToFeaColour(color));
    pickup.overlay.setPosition({position.x * mTileSize.x, position.y * mTileSize.y});
    pickup.overlay.setTexture(mPickupTexture);
    pickup.overlay.setSubrect({additive ? 0.0f : 0.5f, 0.0f}, {additive ? 0.5f : 1.0f, 1.0f});
    pickup.overlay.setSize({mTileSize.x, mTileSize.y});

    return pickup;
}

fea::Color Renderer::glmToFeaColour(const glm::uvec3& col) const
{
    fea::Color realColor;

    realColor.setR(std::max(0, (int32_t)col.r * 64 - 1));
    realColor.setG(std::max(0, (int32_t)col.g * 64 - 1));
    realColor.setB(std::max(0, (int32_t)col.b * 64 - 1));

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
                player.setColor(playerRed);
            }
        if(mAnimationInfo.g == 1)
            for(auto& player : mPlayerColourMeter[GREEN])
            {
                player.setColor(playerGreen);
            }
        if(mAnimationInfo.b == 1)
            for(auto& player : mPlayerColourMeter[BLUE])
            {
                player.setColor(playerBlue);
            }

        if(mAnimationInfo.r == -1)
            for(auto& player : mOverlayMeter[RED])
            {
                player.setColor(playerRed - fea::Color(200, 0, 0, 0));
            }
        if(mAnimationInfo.g == -1)
            for(auto& player : mOverlayMeter[GREEN])
            {
                player.setColor(playerGreen - fea::Color(0, 200, 0, 0));
            }
        if(mAnimationInfo.b == -1)
            for(auto& player : mOverlayMeter[BLUE])
            {
                player.setColor(playerBlue - fea::Color(0, 0, 200, 0));
            }
    }

    if(mAnimationTimer == 45)
    {
        if(mAnimationInfo.r == 1)
            for(auto& player : mPlayerColourMeter[RED])
            {
                player.setColor(playerRed + fea::Color(0, 100, 100));
            }
        if(mAnimationInfo.g == 1)
            for(auto& player : mPlayerColourMeter[GREEN])
            {
                player.setColor(playerGreen + fea::Color(100, 0, 100));
            }
        if(mAnimationInfo.b == 1)
            for(auto& player : mPlayerColourMeter[BLUE])
            {
                player.setColor(playerBlue + fea::Color(100, 100, 0));
            }

        if(mAnimationInfo.r == -1)
            for(auto& player : mOverlayMeter[RED])
            {
                player.setColor(playerRed - fea::Color(230, 0, 0, 0));
            }
        if(mAnimationInfo.g == -1)
            for(auto& player : mOverlayMeter[GREEN])
            {
                player.setColor(playerGreen - fea::Color(0, 230, 0, 0));
            }
        if(mAnimationInfo.b == -1)
            for(auto& player : mOverlayMeter[BLUE])
            {
                player.setColor(playerBlue - fea::Color(0, 0, 230, 0));
            }
    }
}
