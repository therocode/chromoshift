#include "levelloader.hpp"
#include "texturemaker.hpp"
        
LevelLoader::LevelLoader(fea::MessageBus& bus) :
    mBus(bus)
{
}

void LevelLoader::load(const std::string& path)
{
    std::string bgPath = path + "_img.png";
    std::string maskPath = path + "_mask.png";

    fea::Texture bgImage;
    bgImage = makeTexture(bgPath);

    fea::Texture maskImage;
    maskImage = makeTexture(maskPath);

    glm::ivec3 accumulator;
    int32_t accumulatorAmount = 0;

    for(int32_t x = 0; x < bgImage.getSize().x; x++)
    {
        for(int32_t y = 0; y < bgImage.getSize().y; y++)
        {
            if(x == 0 || y == 0 || x == bgImage.getSize().x - 1 || y == bgImage.getSize().y)
            {
                fea::Color colour = bgImage.getPixel(x, y);
                accumulator.r += colour.r();
                accumulator.g += colour.g();
                accumulator.b += colour.b();
                accumulatorAmount++;
            }
        }
    }

    fea::Color average;

    average.setR(accumulator.r / accumulatorAmount);
    average.setG(accumulator.g / accumulatorAmount);
    average.setB(accumulator.b / accumulatorAmount);

    mBus.send(BGMessage{bgImage});
    mBus.send(MaskMessage{maskImage});

    mBus.send(BackgroundColourMessage{average});
}
