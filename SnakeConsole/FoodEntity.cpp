#include "FoodEntity.h"

#include "Snake.h"

FoodEntity::FoodEntity(Vector2Int pos, char charType) : 
BaseEntity(charType),
m_Position(pos)
{
}

char FoodEntity::GetCharType() const
{
    return m_charType;
}

void FoodEntity::SetCharType(char charType)
{
    m_charType = charType;
}

Vector2Int FoodEntity::GetPosition() const
{
    return m_Position;
}

void FoodEntity::SetPosition(Vector2Int pos)
{
    m_Position = pos;
}

bool FoodEntity::Occupies(Vector2Int pos) const
{
    return m_Position == pos;
}

void FoodEntity::RenderToGrid(Grid& grid) const
{
    grid.SetCell(m_Position, CellType::Food);
}

void FoodEntity::SpawnFood(Grid& grid, Snake& snake)
{
    // sets old position to empty
    grid.SetCell(m_Position, CellType::Empty);
    // Randomize the position for spawning in food
    for (int i = 0; i < 200; ++i)
    {
        // Gets a random pos in the grid_area
        int randX {GetRandomNumber(grid.Width()) - 1  };
        int randY {GetRandomNumber(grid.Height()) - 1 };
        
        if (!grid.InBounds(randX, randY) || snake.Occupies(Vector2Int{randX,randY}))
            continue;
        
        Vector2Int newFoodPosition = Vector2Int{randX,randY};
        
        // if the food spawns inside a wall or player it finds a new random place to spawn
        CellType vectorValue { grid.GetCell(newFoodPosition)};
        if (!(vectorValue == CellType::Wall || vectorValue == CellType::Player))
        {
            SetPosition(newFoodPosition);
            return;
        }
    }
    
    // if no free space is found do a liniar search
    for (int y = 0; y < grid.Height(); ++y)
    {
        for (int x = 0; x < grid.Width(); ++x)
        {
            // if the position is not empty
            if (grid.GetCell(Vector2Int{x, y}) != CellType::Empty) continue;
            SetPosition(Vector2Int{x, y});
            return;
        }
    }
    
    // if there is no empty spaces left on the grid, Default to (0,0)
    SetPosition(Vector2Int{0,0});
    
    
}
