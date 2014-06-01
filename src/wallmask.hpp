#pragma once
#include <stdint.h>
#include <vector>
#include <glm/glm.hpp>

class WallMask
{
    public:
        WallMask();
        WallMask(std::vector<bool> mask, uint32_t imageWidth);
        bool isWallAt(glm::uvec2 pos);
    private:
        std::vector<bool> mWallMask;
        uint32_t mImageWidth;
};
