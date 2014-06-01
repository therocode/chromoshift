#include "wallmask.hpp"

WallMask::WallMask()
{
}

WallMask::WallMask(std::vector<bool> mask, uint32_t imageWidth)
{
    mWallMask = mask;
    mImageWidth = imageWidth;
}

bool WallMask::isWallAt(glm::uvec2 pos)
{
    return mWallMask.at((pos.y * mImageWidth) + pos.x);
}
