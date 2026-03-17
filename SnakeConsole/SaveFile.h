#pragma once
#include <string>
#include <vector>
#include <fstream>

#include "SaveData.h"

class SaveFile
{
private:
    const std::string m_fileName;
    const int m_maxTopScoreAmount = 5;
    
    bool IsTopScore(int score);

    static void Sort(std::vector<SaveData>& data);
    
public:
    SaveFile(std::string fileName, int maxTopScoreAmount = 5);
    ~SaveFile() = default;
    
    bool FileExists();
    
    bool SaveHighScoreData(const SaveData& data);
    
    std::vector<SaveData> GetHighScoreData();
};
