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
        const std::string& previous();
        bool hasPrevious() const;
        void reset();
    private:
        int32_t mIterator;
        std::vector<std::string> mLevels;
};
