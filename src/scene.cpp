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
    uint32_t imageSize = wallMaskImage.getSize().x * wallMaskImage.getSize().y;

    std::vector<bool> tempMask;
    tempMask.resize(imageSize, false);

    for(uint32_t i = 0; i < imageSize; i++)
    {
        sf::Color colour = imageArray[i];
        // first two must always be true as these are the player colour pixels
        if(i == 0 || i == 1 ||
            colour == sf::Color::Black)
        {
            tempMask.at(i) = true;
        }
        // also do colour entity stuff
        // 0,1,2,3,4
        else if(colour != sf::Color::Transparent)
        {
            if(colour == sf::Color::White)
            {
                //player entity!
                glm::uvec2 pos = glm::uvec2(i % wallMaskImage.getSize().x, i / wallMaskImage.getSize().x);
                mPlayer->setAttribute("position", pos);
                mBus.send(PlayerPositionMessage(pos));

                glm::uvec3 col = glm::uvec3(colour.r, colour.g, colour.b);
                mPlayer->setAttribute("colour", col);
                mBus.send(PlayerColourMessage(col));
            }
            else
            {
                fea::EntityPtr pickup = mFactory.instantiate("colour_pickup").lock();

                glm::uvec2 pos = glm::uvec2(i % wallMaskImage.getSize().x, i / wallMaskImage.getSize().x);
                pickup->setAttribute("position", pos);

                glm::uvec3 col = glm::uvec3(colour.r, colour.g, colour.b);
                pickup->setAttribute("colour", col);

                bool add = (colour.a == 255) ? true : false;
                pickup->setAttribute("additive", add);

                size_t id = pickup->getId();

                mBus.send(ColourPickupCreatedMessage(id, pos, col, add));
            }
        }
    }

    mWallMask = WallMask(tempMask, wallMaskImage.getSize().x);
}
