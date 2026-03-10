#pragma once
#include <string>
#include <vector>
#include <fstream>

#include "SaveData.h"

class SaveFile
{
public:
    void static Sort(std::vector<SaveData>& data);
    
    bool static FileExists(const std::string& filePath);
    
    bool static SaveHighscoreData(const SaveData data, const std::string& fileName);
    
    std::vector<SaveData> static GetHighscoreData(const std::string& fileName);
};
