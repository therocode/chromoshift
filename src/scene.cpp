#include "scene.hpp"
#include "sounds.hpp"
#include "direction.hpp"
#include <fea/entity/glmtypeadder.hpp>
#include <fea/entity/basictypeadder.hpp>
#include <glm/glm.hpp>

Scene::Scene(fea::MessageBus& bus) :
    mBus(bus),
    mFactory(mManager),
    mIsDead(true)
{
    mBus.addSubscriber<MaskMessage>(*this);
    mBus.addSubscriber<MoveMessage>(*this);

    fea::addGlmDataTypes(mFactory);
    fea::addBasicDataTypes(mFactory);

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
    mIsDead = false;
    processWallMaskImage(mess.maskImage);
}

void Scene::handleMessage(const MoveMessage& mess)
{
    if(mIsDead)
        return;

    Direction dir = mess.direction;

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
    mBus.send(PlayerPositionMessage{newPos});

    // add or subtract from player colour
    fea::EntityPtr pickup = colourPickupAtPosition(newPos);
    if(pickup != nullptr)
    {
        glm::uvec3 playerColour = mPlayer->getAttribute<glm::uvec3>("colour");
        glm::uvec3 pickupColour = pickup->getAttribute<glm::uvec3>("colour");

        bool additive = pickup->getAttribute<bool>("additive");
        additive ?
            (playerColour = playerColour + pickupColour)
          : (playerColour = playerColour - pickupColour);
        
        //check for dying
        if(playerColour.r > 4 || playerColour.g > 4 || playerColour.b > 4)
        {
            glm::ivec3 dieReason((playerColour.r > 5000 ? -1 : 0) + (playerColour.r > 4 && playerColour.r <= 5000 ? 1 : 0),
                                (playerColour.g > 5000 ? -1 : 0) + (playerColour.g > 4 &&  playerColour.g <= 5000 ? 1 : 0),
                                (playerColour.b > 5000 ? -1 : 0) + (playerColour.b > 4 &&  playerColour.b <= 5000 ? 1 : 0));

            mBus.send(PlayerDiedMessage{dieReason});
            mIsDead = true;
        }

        playerColour.r = std::min(4, std::max(0, (int32_t)playerColour.r));
        playerColour.g = std::min(4, std::max(0, (int32_t)playerColour.g));
        playerColour.b = std::min(4, std::max(0, (int32_t)playerColour.b));

        mPlayer->setAttribute("colour", playerColour);
        mBus.send(PlayerColourMessage{playerColour});
        removeColourPickup(pickup->getId());

        if(!mIsDead)
        {
            mBus.send(SoundMessage{additive ? Sound::ADDER : Sound::SUBBER});
        }
        else
        {
            mBus.send(SoundMessage{DIE});
            mBus.send(SongPlayingMessage{false});
        }

        //check for winning
        if(playerColour == mGoalColour)
            mBus.send(LevelSolvedMessage());
    }
}

void Scene::removeColourPickup(size_t id)
{
    mColourPickups.erase(id);
    mManager.removeEntity(id);
    mBus.send(ColourPickupRemovedMessage{id});
}

fea::EntityPtr Scene::colourPickupAtPosition(const glm::uvec2& pos)
{
    fea::EntityPtr tempEntity = nullptr;
    for(auto pickup : mColourPickups)
    {
        glm::uvec2 entityPosition = pickup.second->getAttribute<glm::uvec2>("position");
        if(pos == entityPosition)
        {
            tempEntity = pickup.second;
        }
    }

    return tempEntity;
}

void Scene::processWallMaskImage(const fea::Texture& wallMaskImage)
{
    // clear the pickups
    while(mColourPickups.size() > 0)
    {
        removeColourPickup(mColourPickups.begin()->first);
    }

    const fea::Color* imageArray = (fea::Color*)wallMaskImage.getPixelData();
    uint32_t imageSize = wallMaskImage.getSize().x * wallMaskImage.getSize().y;

    std::vector<bool> tempMask;
    tempMask.resize(imageSize, false);

    fea::Color startColour;
    for(uint32_t i = 0; i < imageSize; i++)
    {
        fea::Color colour = imageArray[i];
        // first two must always be walls as these are the player colour pixels
        if(i == 0)
        {
            startColour = colour;
            tempMask.at(i) = true;
        }
        else if(i == 1)
        {
            mGoalColour = SFToGlmColour(colour);
            mBus.send(GoalColourMessage{mGoalColour});
            tempMask.at(i) = true;
        }
        // setting the other walls
        else if(colour == fea::Color::Black)
        {
            tempMask.at(i) = true;
        }
        else if(colour != fea::Color::Transparent)
        {
            if(colour == fea::Color::White)
            {
                // player entity!
                glm::uvec2 pos = glm::uvec2(i % wallMaskImage.getSize().x, i / wallMaskImage.getSize().x);
                mPlayer->setAttribute("position", pos);
                mBus.send(PlayerPositionMessage{pos});

                glm::uvec3 col = SFToGlmColour(startColour);
                mPlayer->setAttribute("colour", col);
                mBus.send(PlayerColourMessage{col});
            }
            else
            {
                // colour entities
                fea::EntityPtr pickup = mFactory.instantiate("colour_pickup").lock();

                glm::uvec2 pos = glm::uvec2(i % wallMaskImage.getSize().x, i / wallMaskImage.getSize().x);
                pickup->setAttribute("position", pos);

                glm::uvec3 col = SFToGlmColour(colour);
                pickup->setAttribute("colour", col);

                bool add = (colour.a() == 255) ? true : false;
                pickup->setAttribute("additive", add);

                size_t id = pickup->getId();

                mColourPickups.emplace(id, pickup);
                mBus.send(ColourPickupCreatedMessage{id, pos, col, add});
            }
        }
    }

    mWallMask = WallMask(tempMask, wallMaskImage.getSize().x);
}

glm::uvec3 Scene::SFToGlmColour(const fea::Color& colour)
{
    return glm::uvec3(colour.r() / 63, colour.g() / 63, colour.b() / 63);
}
