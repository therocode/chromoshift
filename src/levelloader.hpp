#pragma once
#include "messages.hpp"

class LevelLoader
{
    public:
        LevelLoader(fea::MessageBus& bus);
        void load(const std::string& path);
    private:
        fea::MessageBus& mBus;
};
