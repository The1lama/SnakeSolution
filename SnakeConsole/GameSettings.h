#pragma once
#include <string>

struct GameSettings
{
    const std::string savePath { "SaveFile.txt" };
    
    int width {20};
    int height {10};
    
    char wallChar{'#'};
    char foodChar{'*'};
    char playerChar{'@'};
    char emptyChar{' '};
    
    void SetGridWidth(const int width);
    void SetGridheight(const int _height);
    
    void SetPlayerChar(const char _playerChar);
    void SetWallChar(const char wallChar);
    void SetFoodChar(const char foodChar);
    void SetEmptyChar(const char emptyChar);
};
