#include "wallmask.hpp"

WallMask::WallMask()
{
}

WallMask::WallMask(std::vector<bool> mask, uint32_t imageWidth)
{
    mWallMask = mask;
    mImageWidth = imageWidth;
}

bool WallMask::isWallAt(uint32_t x, uint32_t y)
{
    return mWallMask.at(y * mImageWidth + x);
}
