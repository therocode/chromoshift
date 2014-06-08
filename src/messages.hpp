#pragma once
#include <fea/messaging.hpp>
#include <glm/glm.hpp>
#include "direction.hpp"
#include "sounds.hpp"

namespace fea
{
    class Texture;
    class Color;
}

FEA_DECLARE_MESSAGE(QuitMessage);

FEA_DECLARE_MESSAGE(MoveMessage, Direction direction;);

FEA_DECLARE_MESSAGE(BGMessage, fea::Texture& bgImage;);

FEA_DECLARE_MESSAGE(MaskMessage, const fea::Texture& maskImage;);

FEA_DECLARE_MESSAGE(ResizeMessage, const glm::uvec2& size;);

FEA_DECLARE_MESSAGE(GoalColourMessage, const glm::uvec3& colour;);

FEA_DECLARE_MESSAGE(PlayerPositionMessage, const glm::uvec2& position;);

FEA_DECLARE_MESSAGE(PlayerColourMessage, const glm::uvec3& colour;);

FEA_DECLARE_MESSAGE(ColourPickupCreatedMessage, 
        size_t id; 
        const glm::uvec2& position; 
        const glm::uvec3& colour; 
        bool additive;);

FEA_DECLARE_MESSAGE(ColourPickupRemovedMessage, size_t id;);

FEA_DECLARE_MESSAGE(SoundMessage, Sound sound;);

FEA_DECLARE_MESSAGE(SongPlayingMessage, bool playing;);

FEA_DECLARE_MESSAGE(PlayerDiedMessage, const glm::ivec3& reason;);

FEA_DECLARE_MESSAGE(LevelSolvedMessage);

FEA_DECLARE_MESSAGE(LevelAdvanceMessage, int32_t amount;);

FEA_DECLARE_MESSAGE(LevelRestartMessage);

FEA_DECLARE_MESSAGE(BackgroundColourMessage, const fea::Color& colour;);

FEA_DECLARE_MESSAGE(AnyKeyPressedMessage);
