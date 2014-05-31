#include "scene.hpp"

Scene::Scene(fea::MessageBus& bus)
  : mBus(bus)
{
    // stub
    //processMaskImage();
}

bool Scene::isWallAt(uint32_t x, uint32_t y)
{
    // stub
    return true;
}

bool Scene::isColourEntityAt(uint32_t x, uint32_t y)
{
    // stub
    return false;
}

void Scene::processWallMaskImage(const sf::Image& wallMaskImage)
{
    // stub
}
