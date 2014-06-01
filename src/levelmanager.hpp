#pragma once
#include <vector>
#include <string>

class LevelManager
{
    public:
        LevelManager();
        void loadLevelPack(const std::string& path);
        const std::string& same() const;
        const std::string& next();
        bool hasNext() const;
        void reset();
    private:
        uint32_t mIterator;
        std::vector<std::string> mLevels;
};
