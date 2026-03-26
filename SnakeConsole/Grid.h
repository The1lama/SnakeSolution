#pragma once

#include <vector>

#include "Vector2Int.h"
#include "CellType.h"

class Grid
{
private:
    int m_width {20};
    int m_height {10};
    
    std::vector<Cell> m_gridData {};
    
    char m_wallChar{'#'};
    char m_foodChar{'*'};
    char m_playerChar{'@'};
    char m_emptyChar{' '};
    
public:
    void SetWallChar(const char& character);
    void SetFoodChar(const char& character);
    void SetPlayerChar(const char& character);
    void SetEmptyChar(const char& character);
    void ResetGridCells();

    // class constructor
    Grid(int width, int height, std::vector<Cell>& gridData);
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
    Cell GetCell(const Vector2Int& position) const;
    Cell& GetCellRef(const Vector2Int& position);

    // clears screen
    void GenerateGrid();
    // sets cell
    void SetCell(const Vector2Int& position, const CellType& value);
    
    // renders grid
    void Render() const;
    
};
