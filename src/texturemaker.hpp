#pragma once
#include <iostream>
#include <fea/render2d.hpp>
#include "lodepng.h"

static fea::Texture makeTexture(std::string path)
{
    std::vector<unsigned char> image; //the raw pixels

    uint32_t width, height;

    //decode
    unsigned error = lodepng::decode(image, width, height, path);

    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
    fea::Texture texture;
    texture.create(width, height, &image[0], false, true);
    return texture;
}
