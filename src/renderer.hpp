#pragma once
#include <fea/render2d.hpp>
#include "messages.hpp"
#include <unordered_map>
#include "colour.hpp"

class Pickup
{
    public:
        Pickup();
        fea::Quad rectangle;
        fea::SubrectQuad overlay;
        void tick();
    private:
        int32_t counter;
};

class Renderer :
    public BGMessageReceiver,
    public ResizeMessageReceiver,
    public PlayerPositionMessageReceiver,
    public GoalColourMessageReceiver,
    public PlayerColourMessageReceiver,
    public ColourPickupCreatedMessageReceiver,
    public ColourPickupRemovedMessageReceiver,
    public BackgroundColourMessageReceiver,
    public PlayerDiedMessageReceiver
{
    public:
        Renderer(fea::MessageBus& b, fea::Renderer2D& r);
        ~Renderer();
        void handleMessage(const BGMessage& message) override;
        void handleMessage(const ResizeMessage& message) override;
        void handleMessage(const PlayerPositionMessage& message) override;
        void handleMessage(const GoalColourMessage& message) override;
        void handleMessage(const PlayerColourMessage& message) override;
        void handleMessage(const ColourPickupCreatedMessage& message) override;
        void handleMessage(const ColourPickupRemovedMessage& message) override;
        void handleMessage(const BackgroundColourMessage& message) override;
        void handleMessage(const PlayerDiedMessage& message) override;
        void render();
    private:
        Pickup createPickup(const glm::uvec2& position, const glm::uvec3& color, bool additive);
        fea::Color glmToFeaColour(const glm::uvec3& col) const;
        void updateInterface();
        fea::MessageBus& mBus;
        fea::Renderer2D& mRenderer;

        fea::Color mBackgroundColor;

        glm::vec2 mTileSize;

        // scene stuff //
        fea::Texture mBgTexture;
        fea::Quad mBackground;

        fea::Quad mPlayer;

        fea::Texture mPickupTexture;
        std::unordered_map<size_t, Pickup> mPickups;

        // interface stuff //
        fea::Texture mInterfaceTexture;
        fea::Texture mInterfaceOverlayTexture;
        fea::Quad mInterfaceSprite;
        fea::Quad mInterfaceOverlaySprite;
        glm::uvec2 mInterfacePosition;
        glm::uvec3 mGoalColour;
        glm::uvec3 mPlayerColour;
        std::unordered_map<Colour, std::vector<fea::Quad>> mGoalColourMeter;
        std::unordered_map<Colour, std::vector<fea::Quad>> mPlayerColourMeter;
        std::unordered_map<Colour, std::vector<fea::Quad>> mOverlayMeter;
        glm::ivec3 mAnimationInfo;
        uint32_t mAnimationTimer;
};
