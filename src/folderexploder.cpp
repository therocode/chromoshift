#include "folderexploder.hpp"
#include "tinydir.h"
#include <regex>
#include <algorithm>

void FolderExploder::explodeFolder(const std::string& directory, const std::string regex, std::vector<std::string>& result)
{
    std::regex rx(regex);
    
    tinydir_dir dir;

    tinydir_open(&dir, directory.c_str());

    while(dir.has_next)
    {
        tinydir_file file;
        tinydir_readfile(&dir, &file);

        std::string fileName = file.name;
        
        if(regex_match(fileName, rx))
        {
            result.push_back(directory + "/" + file.name);
        }

        if(file.is_dir)
        {
            if(std::string(file.name) != "." && std::string(file.name) != "..")
            {
                explodeFolder(directory + "/" + file.name, regex, result);
            }
        }
        else
        {

        }

        tinydir_next(&dir);
    }

    std::sort(result.begin(), result.end());
}
