#pragma once
#include "messages.hpp"
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "colour.hpp"

class Pickup
{
    public:
        Pickup();
        sf::RectangleShape rectangle;
        sf::Sprite overlay;
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
        Renderer(fea::MessageBus& b, sf::RenderWindow& w);
        ~Renderer();
        virtual void handleMessage(const BGMessage& message) override;
        virtual void handleMessage(const ResizeMessage& message) override;
        virtual void handleMessage(const PlayerPositionMessage& message) override;
        virtual void handleMessage(const GoalColourMessage& message) override;
        virtual void handleMessage(const PlayerColourMessage& message) override;
        virtual void handleMessage(const ColourPickupCreatedMessage& message) override;
        virtual void handleMessage(const ColourPickupRemovedMessage& message) override;
        virtual void handleMessage(const BackgroundColourMessage& message) override;
        void handleMessage(const PlayerDiedMessage& message) override;
        void render();
    private:
        Pickup createPickup(const glm::uvec2& position, const glm::uvec3& color, bool additive);
        sf::Color glmToSFColour(const glm::uvec3& col) const;
        void updateInterface();
        fea::MessageBus& mBus;
        sf::RenderWindow& mWindow;

        sf::Color mBackgroundColor;

        glm::vec2 mTileSize;

        // scene stuff //
        sf::View mSceneView;
        sf::Texture mBgTexture;
        sf::Sprite mBackground;

        sf::RectangleShape mPlayer;

        sf::Texture mPickupTexture;
        std::unordered_map<size_t, Pickup> mPickups;

        // interface stuff //
        sf::View mInterfaceView;
        sf::Texture mInterfaceTexture;
        sf::Texture mInterfaceOverlayTexture;
        sf::Sprite mInterfaceSprite;
        sf::Sprite mInterfaceOverlaySprite;
        glm::uvec2 mInterfacePosition;
        glm::uvec3 mGoalColour;
        glm::uvec3 mPlayerColour;
        std::unordered_map<Colour, std::vector<sf::RectangleShape>> mGoalColourMeter;
        std::unordered_map<Colour, std::vector<sf::RectangleShape>> mPlayerColourMeter;
        std::unordered_map<Colour, std::vector<sf::RectangleShape>> mOverlayMeter;
        glm::ivec3 mAnimationInfo;
        uint32_t mAnimationTimer;
};
