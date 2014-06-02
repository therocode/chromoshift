#pragma once
#include "messages.hpp"
#include <unordered_map>
#include <SFML/Graphics.hpp>

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
    public BackgroundColourMessageReceiver
{
    public:
        Renderer(fea::MessageBus& b, sf::RenderWindow& w);
        ~Renderer();
        void handleMessage(const BGMessage& message) override;
        void handleMessage(const ResizeMessage& message) override;
        void handleMessage(const PlayerPositionMessage& message) override;
        void handleMessage(const GoalColourMessage& message) override;
        void handleMessage(const PlayerColourMessage& message) override;
        void handleMessage(const ColourPickupCreatedMessage& message) override;
        void handleMessage(const ColourPickupRemovedMessage& message) override;
        void handleMessage(const BackgroundColourMessage& message) override;
        void render();
    private:
        Pickup createPickup(const glm::uvec2& position, const glm::uvec3& color, bool additive);
        sf::Color glmToSFColour(const glm::uvec3& col) const;
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
        std::vector<sf::RectangleShape> mGoalColourMeter;
        std::vector<sf::RectangleShape> mPlayerColourMeter;
};
