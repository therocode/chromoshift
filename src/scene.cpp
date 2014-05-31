#include "scene.hpp"

Scene::Scene(fea::MessageBus& bus)
  : mBus(bus)
{
    mBus.addSubscriber<MaskMessage>(*this);
}

Scene::~Scene()
{
    mBus.removeSubscriber<MaskMessage>(*this);
}

void Scene::handleMessage(const MaskMessage& mess)
{
    processWallMaskImage(std::get<0>(mess.mData));
}

bool Scene::isWallAt(uint32_t x, uint32_t y)
{
    return mWallMask.isWallAt(x, y);
}

bool Scene::isColourEntityAt(uint32_t x, uint32_t y)
{
    // stub
    return false;
}

void Scene::processWallMaskImage(const sf::Image& wallMaskImage)
{
    const sf::Color* imageArray = (sf::Color*)wallMaskImage.getPixelsPtr();
    uint32_t imageSize = wallMaskImage.getSize().x * wallMaskImage.getSize().y * 4;

    std::vector<bool> tempMask;
    tempMask.resize(imageSize, false);

    for(uint32_t i = 0; i < imageSize/4; i++)
    {
        sf::Color colour = imageArray[i];
        // first two must always be true as these are the player colour pixels
        if(i == 0 || i == 1 ||
            colour == sf::Color::Black)
        {
            tempMask.at(i) = true;
        }
        // also do colour entity stuff
    }

    mWallMask = WallMask(tempMask, wallMaskImage.getSize().x);
}
