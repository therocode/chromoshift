#pragma once
#include <SFML/Graphics.hpp>
#include <fea/entitysystem.hpp>
#include "messages.hpp"
#include "wallmask.hpp"

class Scene
  : public fea::MessageReceiver<MaskMessage>,
    public fea::MessageReceiver<MoveMessage>
{
    public:
        Scene(fea::MessageBus& bus);
        ~Scene();

        bool isWallAt(glm::uvec2 pos);
        bool isColourEntityAt(glm::uvec2 pos);

        void handleMessage(const MaskMessage& mess);
        void handleMessage(const MoveMessage& mess);

    private:
        fea::MessageBus& mBus;
        fea::EntityManager mManager;
        fea::EntityFactory mFactory;
        fea::EntityPtr mPlayer; 
        std::vector<fea::EntityPtr> mColourPickups; 
        WallMask mWallMask;

        void processWallMaskImage(const sf::Image& wallMaskImage);
        glm::uvec3 SFToGlmColour(const sf::Color& colour);
};
