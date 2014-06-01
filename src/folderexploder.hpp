#pragma once
#include <vector>
#include <string>

//regex example:    
//exploder.explodeFolder("data", ".*\\.as|.*\\.def", sourceFiles);

class FolderExploder
{
    public:
        void explodeFolder(const std::string& directory, const std::string regex, std::vector<std::string>& result);
};
