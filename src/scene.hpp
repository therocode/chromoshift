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

        bool isWallAt(uint32_t x, uint32_t y);
        bool isColourEntityAt(uint32_t x, uint32_t y);

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
};
