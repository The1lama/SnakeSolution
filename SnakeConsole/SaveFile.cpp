#include "SaveFile.h"


// a simple bubble sort. For sorting the save file data
void SaveFile::Sort(std::vector<SaveData>& data)
{
    for (int i=0; i<data.size()-1; ++i)
    {
        for (int j=0; j<data.size()-i-1; ++j)
        {
            if (data[j].score < data[j+1].score)
            {
                SaveData tempData = data[j];
                data[j] = data[j+1];
                data[j+1] = tempData;   
            }
        }
    }
}

// Check if the save file exists on disk
bool SaveFile::FileExists(const std::string& filePath)
{
    return std::ifstream{filePath}.good();
}

bool SaveFile::SaveHighscoreData(const SaveData data, const std::string& fileName)
{
    std::ofstream outf{ fileName, std::ios::app };
    // if file could not be opened
    if (!outf)
        return false;
    
    outf << data.score << 
        ' ' << data.playerName << '\n';
    outf.close();
    return true;
}

std::vector<SaveData> SaveFile::GetHighscoreData(const std::string& filePath)
{
    if (!FileExists(filePath))
    {
        std::ofstream outf{ filePath };
        outf.close();
    }
    
    std::ifstream inf{ filePath };
    
    if (!inf)
        return {};
    
    std::vector<SaveData> savedPlays;
    std::vector<std::string> strings;
    std::string strInput{};
    while (inf >> strInput)
    {
        strings.push_back(strInput);
        strInput.clear();
    }
    for (int i = 0; i < strings.size(); i+=2)
    {
        savedPlays.push_back(SaveData{ strings[i+1], std::stoi(strings[i]) });
    }
    Sort(savedPlays);
    return savedPlays;
}