#include "scene.hpp"

Scene::Scene(fea::MessageBus& bus)
  : mBus(bus)
{
    // stub
    //processWallMaskImage(bildibild);
}

bool Scene::isWallAt(uint32_t x, uint32_t y)
{
    // stub
    wallMask.isWallAt(x, y);
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
    const uint8_t* imageArray = wallMaskImage.getPixelsPtr();
    uint32_t imageSize = wallMaskImage.getSize().x * wallMaskImage.getSize().y * 4;
    for(uint32_t i = 0; i < imageSize; i += 4)
    {
        // do nothing
    }
}
