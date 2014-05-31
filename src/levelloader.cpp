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
}
