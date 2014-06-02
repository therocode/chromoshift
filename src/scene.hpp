#pragma once
#include <fea/entitysystem.hpp>
#include <fea/render2d.hpp>
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
        glm::uvec3 mGoalColour;

        void processWallMaskImage(const fea::Texture& wallMaskImage);
        glm::uvec3 SFToGlmColour(const fea::Color& colour);
        fea::EntityPtr colourPickupAtPosition(const glm::uvec2& pos);
        void removeColourPickup(size_t id);

        bool mIsDead;
};
