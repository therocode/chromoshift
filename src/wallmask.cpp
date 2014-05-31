#include "wallmask.hpp"

WallMask::WallMask()
{
    //stub
}

bool WallMask::isWallAt(uint32_t x, uint32_t y)
{
    return wallMask.at(y * imageLength + x);
}
