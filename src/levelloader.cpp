#include "levelloader.hpp"
#include <SFML/Graphics.hpp>
        
LevelLoader::LevelLoader(fea::MessageBus& bus) :
    mBus(bus)
{
}

void LevelLoader::load(const std::string& path)
{
    std::string bgPath = path + "_img.png";
    std::string maskPath = path + "_mask.png";

    sf::Image bgImage;
    bgImage.loadFromFile(bgPath);

    sf::Image maskImage;
    maskImage.loadFromFile(maskPath);

    mBus.send(BGMessage(bgImage));
    mBus.send(MaskMessage(maskImage));

    glm::ivec3 accumulator;
    int32_t accumulatorAmount = 0;

    for(int32_t x = 0; x < bgImage.getSize().x; x++)
    {
        for(int32_t y = 0; y < bgImage.getSize().y; y++)
        {
            if(x == 0 || y == 0 || x == bgImage.getSize().x - 1 || y == bgImage.getSize().y)
            {
                sf::Color colour = bgImage.getPixel(x, y);
                accumulator.r += colour.r;
                accumulator.g += colour.g;
                accumulator.b += colour.b;
                accumulatorAmount++;
            }
        }
    }

    sf::Color average;

    average.r = accumulator.r / accumulatorAmount;
    average.g = accumulator.g / accumulatorAmount;
    average.b = accumulator.b / accumulatorAmount;

    mBus.send(BackgroundColourMessage(average));
}
