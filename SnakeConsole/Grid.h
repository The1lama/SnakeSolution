#pragma once

#include <Windows.h>
#include <iostream>
#include <vector>

#include "Vector2Int.h"
#include "CellType.h"
#include "Direction.h"


class Grid
{
private:
    int m_width {20};
    int m_height {10};
    
    std::vector<CellType> m_gridData {};
    
    char m_wallChar{'#'};
    char m_foodChar{'*'};
    char m_playerChar{'@'};
    char m_emptyChar{' '};
    
public:
    void SetWallChar(const char& character);
    void SetFoodChar(const char& character);
    void SetPlayerChar(const char& character);
    void SetEmptyChar(const char& character);
    
    // class constructor
    Grid(int width, int height);
    // class deconstructor
    ~Grid();
    
    // used for getting the width and height of the play area
    int Width() const;
    int Height() const;
    
    // check is the cell is inbounds
    bool InBounds(const Vector2Int& position) const;
    bool InBounds(const int& x, const int& y) const;
    
    // get position to index in array
    int ToIndex(const Vector2Int& position) const;   // y*width+x
    int ToIndex(const int& x, const int& y) const;
    CellType GetCell(const Vector2Int& position) const;
    
    // clears screen
    void Clear();
    // sets cell
    void SetCell(const Vector2Int& position, const CellType value);
    
    // renders grid
    void Render() const;
};
