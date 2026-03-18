#pragma once
#include <string>

struct SaveData
{
    std::string playerName;
    int score;
    
    SaveData() = default;
    SaveData(const std::string playerName, const int score);
    
};
