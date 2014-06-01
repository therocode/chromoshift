#pragma once
#include <fea/messaging.hpp>
#include <glm/glm.hpp>
#include "direction.hpp"
#include "sounds.hpp"

namespace sf
{
    class Image;
    class Color;
}

FEA_DECLARE_MESSAGE(QuitMessage);
                                // enum direction
FEA_DECLARE_MESSAGE(MoveMessage, Direction);
                                // bg image
FEA_DECLARE_MESSAGE(BGMessage, const sf::Image&);
                                // mask image
FEA_DECLARE_MESSAGE(MaskMessage, const sf::Image&);
                                // screen size
FEA_DECLARE_MESSAGE(ResizeMessage, const glm::uvec2&);
                                //     colour
FEA_DECLARE_MESSAGE(GoalColourMessage, const glm::uvec3&);
                                //         position
FEA_DECLARE_MESSAGE(PlayerPositionMessage, const glm::uvec2&);
                                //       colour
FEA_DECLARE_MESSAGE(PlayerColourMessage, const glm::uvec3&);
                                //              id,     position           colour             additive
FEA_DECLARE_MESSAGE(ColourPickupCreatedMessage, size_t, const glm::uvec2&, const glm::uvec3&, bool);
                                //              id
FEA_DECLARE_MESSAGE(ColourPickupRemovedMessage, size_t);
                                // sound
FEA_DECLARE_MESSAGE(SoundMessage, Sound);
