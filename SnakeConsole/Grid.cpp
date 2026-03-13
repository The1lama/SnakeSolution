#include "Grid.h"



// class constructor, this is for creating the play area size
Grid::Grid(const int width, const int height) : 
    m_width(width),
    m_height(height),
    m_gridData(width*height)
{
    Clear();
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
    if (!(x > 0 && x < m_width-1 && y > 0 && y < m_height-1))
        return false;
    return GetCell(Vector2Int{x, y}) != CellType::Wall;
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

void Grid::Clear()
{
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            if (x == 0 || x == m_width-1 || y == 0 || y == m_height-1)    // sets the cell type to wall 
                m_gridData[y * m_width + x] = CellType::Wall;
            else
                m_gridData[y * m_width + x] = CellType::Empty;
        }
    }
}

void Grid::SetCell(const Vector2Int& position, const CellType value)
{
    int index = ToIndex(position);
    m_gridData[index] = value;
}

CellType Grid::GetCell(const Vector2Int& position) const
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
            switch (m_gridData[ToIndex(x, y)])
            {
            case CellType::Wall:
                std::cout << m_wallChar;
                continue;
            case CellType::Empty:
                std::cout << m_emptyChar;
                continue;
            case CellType::Food:
                std::cout << m_foodChar;
                continue;
            case CellType::Player:
                std::cout << m_playerChar;
                continue;
            }
        }
        std::cout << '\n';
    }
}

std::vector<CellType>& Grid::GetGridData()
{
    return m_gridData;
}
