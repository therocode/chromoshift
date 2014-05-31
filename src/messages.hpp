#pragma once
#include <fea/messaging.hpp>
#include "direction.hpp"

namespace sf
{
    class Image;
}

FEA_DECLARE_MESSAGE(QuitMessage);
FEA_DECLARE_MESSAGE(MoveMessage, Direction);
FEA_DECLARE_MESSAGE(BGMessage, const sf::Image&);
FEA_DECLARE_MESSAGE(MaskMessage, const sf::Image&);
FEA_DECLARE_MESSAGE(ResizeMessage, uint32_t, uint32_t);
