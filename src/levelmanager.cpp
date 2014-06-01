#include "levelmanager.hpp"
#include "folderexploder.hpp"
#include <iostream>

LevelManager::LevelManager() : mIterator(0)
{
}

void LevelManager::loadLevelPack(const std::string& path)
{
    FolderExploder exploder;

    exploder.explodeFolder(path.c_str(), ".*mask.png", mLevels);

    for(auto& level : mLevels)
    {
        level.pop_back();
        level.pop_back();
        level.pop_back();
        level.pop_back();
        level.pop_back();
        level.pop_back();
        level.pop_back();
        level.pop_back();
        level.pop_back();
    }
}

const std::string& LevelManager::same() const
{
    return mLevels[mIterator];
}

const std::string& LevelManager::next()
{
    mIterator++;
    return mLevels[mIterator];
}

bool LevelManager::hasNext() const
{
    return mIterator + 1 < mLevels.size();
}

void LevelManager::reset()
{
    mIterator = 0;
}
