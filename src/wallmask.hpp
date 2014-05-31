#pragma once
#include <stdint.h>
#include <vector>

class WallMask
{
    public:
        WallMask();
        WallMask(std::vector<bool> mask, uint32_t imageWidth);
        bool isWallAt(uint32_t x, uint32_t y);
    private:
        std::vector<bool> mWallMask;
        uint32_t mImageWidth;
};
