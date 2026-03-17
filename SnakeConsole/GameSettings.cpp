#include "GameSettings.h"

GameSettings::GameSettings(int Width, int Height) : 
width(Width),
height(Height)
{
}


// Sets the grid Width
void GameSettings::SetGridWidth(const int _width)
{
    width = _width;
}
// Sets the grid height
void GameSettings::SetGridheight(const int _height)
{
    height = _height;
}
// Sets player character
void GameSettings::SetPlayerChar(const char _playerChar)
{
    playerChar = _playerChar;
}
// Sets Wall character
void GameSettings::SetWallChar(const char _wallChar)
{
    wallChar = _wallChar;
}
// Sets Food character
void GameSettings::SetFoodChar(const char _foodChar)
{
    foodChar = _foodChar;
}
// Sets Empty character
void GameSettings::SetEmptyChar(const char _emptyChar)
{
    emptyChar = _emptyChar;
}
