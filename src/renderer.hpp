#pragma once
#include "messages.hpp"
#include <unordered_map>
#include <SFML/Graphics.hpp>

struct Pickup
{
    sf::RectangleShape rectangle;
    sf::Sprite overlay;
};

class Renderer :
    public BGMessageReceiver,
    public ResizeMessageReceiver,
    public PlayerPositionMessageReceiver,
    public PlayerColourMessageReceiver,
    public ColourPickupCreatedMessageReceiver
{
    public:
        Renderer(fea::MessageBus& b, sf::RenderWindow& w);
        ~Renderer();
        virtual void handleMessage(const BGMessage& message) override;
        virtual void handleMessage(const ResizeMessage& message) override;
        virtual void handleMessage(const PlayerPositionMessage& message) override;
        virtual void handleMessage(const PlayerColourMessage& message) override;
        virtual void handleMessage(const ColourPickupCreatedMessage& message) override;
        void render();
    private:
        Pickup createPickup(const glm::uvec2& position, const glm::uvec3& color, bool additive);
        sf::Color glmToSFColor(const glm::uvec3& col) const;
        fea::MessageBus& mBus;
        sf::RenderWindow& mWindow;

        sf::Texture mBgTexture;
        sf::Sprite mBackground;

        sf::RectangleShape mPlayer;

        sf::Texture mPickupTexture;
        std::unordered_map<size_t, Pickup> mPickups;
};
