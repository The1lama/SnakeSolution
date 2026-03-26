#include "Grid.h"

#include <iostream>
#include <ranges>
#include <Windows.h>

// class constructor, this is for creating the play area size
Grid::Grid(const int width, const int height, std::vector<Cell>& gridData) : 
    m_width(width),
    m_height(height),
    m_gridData(gridData)
{
}
// class deconstructor, this is for when this class gets deconstructed so we dont leave any 
// memory leaks
Grid::~Grid()
{
}

void Grid::SetWallChar(const char& character)
{
    m_wallChar = character;
}
void Grid::SetFoodChar(const char& character)
{
    m_foodChar = character;
}
void Grid::SetPlayerChar(const char& character)
{
    m_playerChar = character;
}
void Grid::SetEmptyChar(const char& character)
{
    m_emptyChar = character;
}

// Resets the grid cell values for the Astar algorithm
void Grid::ResetGridCells()
{
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            m_gridData[ToIndex(x, y)].ResetCellValue();
        }
    }
}

// Get the Width of the grid
int Grid::Width() const
{
    return m_width;
}
// Get the Height of the grid
int Grid::Height() const
{
    return m_height;
}

// check is the cell inside the grid
bool Grid::InBounds(const Vector2Int& position) const
{
    return InBounds(position.x, position.y);
}
bool Grid::InBounds(const int& x, const int& y) const
{
    if (!(x >= 0 && x <= m_width-1 && y >= 0 && y <= m_height-1))
        return false;
    return GetCell(Vector2Int{x, y}).type != CellType::Wall;
}

// get the index in grid by position
int Grid::ToIndex(const Vector2Int& position) const
{
    return position.y * m_width + position.x;
}
int Grid::ToIndex(const int& x, const int& y) const
{
    return y * m_width + x;
}

void Grid::GenerateGrid()
{
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            int index = ToIndex(x, y);
            if (x == 0 || x == m_width-1 || y == 0 || y == m_height-1)    // sets the cell type to wall 
                m_gridData[index] = Cell{CellType::Wall, Vector2Int{x, y}};
            else
                m_gridData[ToIndex(x,y)] = Cell{CellType::Empty, Vector2Int{x, y}};
        }
    }
}

void Grid::SetCell(const Vector2Int& position, const CellType& value)
{
    int index = ToIndex(position);
    m_gridData[index].type = value;
}

Cell Grid::GetCell(const Vector2Int& position) const
{
    return m_gridData[ToIndex(position)];
}
Cell& Grid::GetCellRef(const Vector2Int& position) 
{
    return m_gridData[ToIndex(position)];
}

void Grid::Render() const
{
    // Clear screen
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{0,0});
    
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            // Get the cell type 
            switch (m_gridData[ToIndex(x, y)].type)
            {
            case CellType::Wall:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                std::cout << m_wallChar;
                continue;
            case CellType::Empty:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
                std::cout << m_emptyChar;
                continue;
            case CellType::Food:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
                std::cout << m_foodChar;
                continue;
            case CellType::Player:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                std::cout << m_playerChar;
                continue;
            }
        }
        std::cout << '\n';
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
