#include "SaveFile.h"


////// PRIVATE //////

// a simple bubble sort. For sorting the save file data
void SaveFile::Sort(std::vector<SaveData>& data)
{
    if (data.empty())
        return;
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

bool SaveFile::IsTopScore(int score)
{
    std::vector<SaveData> data = GetHighScoreData();
    
    // if there is less than maxTopScoreAmount it is valid for top placement
    if (static_cast<int>(data.size()) < m_maxTopScoreAmount)
        return true;
    
    // check the last score of the saved scores is lower than the new score 
    // it is valid for top score
    int lowestHighScore = data.back().score;
    if (lowestHighScore < score)
        return true;
    
    // if its not a topscorer
    return false;
}

////// PUBLIC //////
SaveFile::SaveFile(std::string fileName, int maxTopScoreAmount) : 
    m_fileName(fileName), 
    m_maxTopScoreAmount(maxTopScoreAmount)
{
}

// Check if the save file exists on disk
bool SaveFile::FileExists() const
{
    return std::ifstream{m_fileName}.good();
}

bool SaveFile::SaveHighScoreData(const SaveData& data)
{
    if (!IsTopScore(data.score))
        return false;
    
    std::vector<SaveData> savedPlays = GetHighScoreData();
    savedPlays.push_back(data);
    
    Sort(savedPlays);
    
    if (savedPlays.size() > m_maxTopScoreAmount)
        savedPlays.resize(m_maxTopScoreAmount);
    

    std::ofstream outf{ m_fileName };
    if (!outf)
        return false;

    for (auto playData : savedPlays)
    {
        outf << playData.score << 
            ' ' << playData.playerName << '\n';
    }
    
    outf.close();
    return true;
}

std::vector<SaveData> SaveFile::GetHighScoreData() const
{
    if (!FileExists())
    {
        std::ofstream outf{ m_fileName };
        outf.close();
    }
    
    std::ifstream inf{ m_fileName };
    
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
