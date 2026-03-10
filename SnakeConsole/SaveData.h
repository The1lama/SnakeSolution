#pragma once
#include <string>

struct SaveData
{
    SaveData(const std::string playerName, const int score);
    
    
    std::string playerName;
    int score;
};
