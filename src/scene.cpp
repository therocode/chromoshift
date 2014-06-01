#include "scene.hpp"
#include "direction.hpp"
#include <fea/util/entity/glmtypeadder.hpp>
#include <fea/util/entity/basictypeadder.hpp>
#include <glm/glm.hpp>

Scene::Scene(fea::MessageBus& bus)
  : mBus(bus),
    mFactory(mManager)
{
    mBus.addSubscriber<MaskMessage>(*this);
    mBus.addSubscriber<MoveMessage>(*this);

    fea::util::addGlmDataTypes(mFactory);
    fea::util::addBasicDataTypes(mFactory);

    mFactory.registerAttribute("position", "uvec2");
    mFactory.registerAttribute("colour"  , "uvec3");
    mFactory.registerAttribute("additive", "bool" );

    fea::EntityTemplate playerEntityTemplate;
    playerEntityTemplate.mAttributes = {{"position", "0, 0"    },
                                        {"colour"  , "0, 0, 0" }};

    mFactory.addTemplate("player", playerEntityTemplate);

    fea::EntityTemplate colourPickupTemplate;
    colourPickupTemplate.mAttributes = {{"position", "0, 0"   },
                                        {"colour"  , "0, 0, 0"},
                                        {"additive", "true"   }};

    mFactory.addTemplate("colour_pickup", colourPickupTemplate);

    mPlayer = mFactory.instantiate("player").lock();
}

Scene::~Scene()
{
    mBus.removeSubscriber<MaskMessage>(*this);
    mBus.removeSubscriber<MoveMessage>(*this);
}

void Scene::handleMessage(const MaskMessage& mess)
{
    processWallMaskImage(std::get<0>(mess.mData));
}

void Scene::handleMessage(const MoveMessage& mess)
{
    Direction dir;
    std::tie(dir) = mess.mData;

    glm::uvec2 newPos;
    glm::uvec2 oldPos = mPlayer->getAttribute<glm::uvec2>("position");
    if(dir == Direction::LEFT)
    {
        newPos = oldPos + glm::uvec2(-1, 0);
    }
    else if(dir == Direction::RIGHT)
    {
        newPos = oldPos + glm::uvec2(1, 0);
    }
    else if(dir == Direction::UP)
    {
        newPos = oldPos + glm::uvec2(0, -1);
    }
    else if(dir == Direction::DOWN)
    {
        newPos = oldPos + glm::uvec2(0, 1);
    }

    // pretto-ternary (boundary checking)
    newPos = mWallMask.isWallAt(newPos) ? oldPos : newPos;

    mPlayer->setAttribute("position", newPos);
    mBus.send(PlayerPositionMessage(newPos));
}

/*  if necessary, make private
bool Scene::isColourEntityAt(glm::uvec2 pos)
{
    // stub
    return false;
}
*/

void Scene::processWallMaskImage(const sf::Image& wallMaskImage)
{
    const sf::Color* imageArray = (sf::Color*)wallMaskImage.getPixelsPtr();
    uint32_t imageSize = wallMaskImage.getSize().x * wallMaskImage.getSize().y;

    std::vector<bool> tempMask;
    tempMask.resize(imageSize, false);

    sf::Color startColour;
    sf::Color goalColour;

    for(uint32_t i = 0; i < imageSize; i++)
    {
        sf::Color colour = imageArray[i];
        // first two must always be walls as these are the player colour pixels
        if(i == 0)
        {
            startColour = colour;
            tempMask.at(i) = true;
        }
        else if(i == 1)
        {
            goalColour = colour;
            tempMask.at(i) = true;
        }
        // setting the other walls
        else if(colour == sf::Color::Black)
        {
            tempMask.at(i) = true;
        }
        else if(colour != sf::Color::Transparent)
        {
            if(colour == sf::Color::White)
            {
                // player entity!
                glm::uvec2 pos = glm::uvec2(i % wallMaskImage.getSize().x, i / wallMaskImage.getSize().x);
                mPlayer->setAttribute("position", pos);
                mBus.send(PlayerPositionMessage(pos));

                glm::uvec3 col = SFToGlmColour(startColour);
                mPlayer->setAttribute("colour", col);
                mBus.send(PlayerColourMessage(col));
            }
            else
            {
                // colour entities
                fea::EntityPtr pickup = mFactory.instantiate("colour_pickup").lock();

                glm::uvec2 pos = glm::uvec2(i % wallMaskImage.getSize().x, i / wallMaskImage.getSize().x);
                pickup->setAttribute("position", pos);

                glm::uvec3 col = SFToGlmColour(colour);
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

glm::uvec3 Scene::SFToGlmColour(const sf::Color& colour)
{
    return glm::uvec3(colour.r / 63, colour.g / 63, colour.b / 63);
}
