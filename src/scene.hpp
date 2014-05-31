#pragma once
#include <SFML/Graphics.hpp>
#include <fea/entity/entity.hpp>
#include "messages.hpp"
#include "wallmask.hpp"

class Scene
{
    public:
        Scene(fea::MessageBus& bus);

        bool isWallAt(uint32_t x, uint32_t y);
        bool isColourEntityAt(uint32_t x, uint32_t y);

    private:
        fea::MessageBus& mBus;
        fea::EntityPtr player; // position and colour value
        std::vector<fea::EntityPtr> colourBlocks; // each have a position and colour value and add/sub
        WallMask wallMask;

        void processWallMaskImage(const sf::Image& wallMaskImage);
};