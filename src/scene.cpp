#include "scene.hpp"
#include <fea/util/entity/glmtypeadder.hpp>
#include <fea/util/entity/basictypeadder.hpp>
#include <glm/glm.hpp>

Scene::Scene(fea::MessageBus& bus)
  : mBus(bus),
    mFactory(mManager)
{
    mBus.addSubscriber<MaskMessage>(*this);

    fea::util::addGlmDataTypes(mFactory);
    fea::util::addBasicDataTypes(mFactory);

    mFactory.registerAttribute("position", "uvec2");
    mFactory.registerAttribute("colour", "uvec3");
    mFactory.registerAttribute("additive", "bool");

    fea::EntityTemplate playerEntityTemplate;
    playerEntityTemplate.mAttributes = {{"position"     , "0, 0" },
        {"colour"     , "0, 0, 0" }};

    mFactory.addTemplate("player", playerEntityTemplate);

    fea::EntityTemplate colourPickupTemplate;
    colourPickupTemplate.mAttributes = {{"position"     , "0, 0" },
        {"colour"     , "0, 0, 0" },
        {"additive"  , "true"    }};

    mFactory.addTemplate("colour_pickup", colourPickupTemplate);

    mPlayer = mFactory.instantiate("player").lock();
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

    for(uint32_t i = 0; i < imageSize; i++)
    {
        std::cout << "\n\n";
        sf::Color colour = imageArray[i];
        // first two must always be true as these are the player colour pixels
        if(i == 0 || i == 1 ||
            colour == sf::Color::Black)
        {
            tempMask.at(i) = true;
            std::cout << "true, ";
        }
        else
        {
            std::cout << "false, ";
        }
        // also do colour entity stuff
        std::cout << "\n\n";
    }

    mWallMask = WallMask(tempMask, wallMaskImage.getSize().x);
}
