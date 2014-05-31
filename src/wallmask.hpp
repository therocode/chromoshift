#pragma once
#include <stdint.h>
#include <vector>

class WallMask
{
    public:
        WallMask();
        bool isWallAt(uint32_t x, uint32_t y);
    private:
        std::vector<bool> wallMask;
        uint32_t imageLength;
};
