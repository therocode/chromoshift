#pragma once
#include <SFML/Graphics.hpp>
#include <fea/entitysystem.hpp>
#include "messages.hpp"
#include "wallmask.hpp"

class Scene :
    public fea::MessageReceiver<MaskMessage>,
    public fea::MessageReceiver<MoveMessage>
{
    public:
        Scene(fea::MessageBus& bus);
        ~Scene();

        void handleMessage(const MaskMessage& mess);
        void handleMessage(const MoveMessage& mess);

    private:
        fea::MessageBus& mBus;
        fea::EntityManager mManager;
        fea::EntityFactory mFactory;
        fea::EntityPtr mPlayer; 
        // the colours within the colour pickups and player are in 0,1,2,3,4 form
        std::unordered_map<size_t, fea::EntityPtr> mColourPickups;
        WallMask mWallMask;

        void processWallMaskImage(const sf::Image& wallMaskImage);
        glm::uvec3 SFToGlmColour(const sf::Color& colour);
        fea::EntityPtr colourPickupAtPosition(const glm::uvec2& pos);
        void removeColourPickup(size_t id);
};
