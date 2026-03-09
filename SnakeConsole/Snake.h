#pragma once
#include <deque>

#include "Direction.h"
#include "Grid.h"
#include "Vector2Int.h"

class Snake
{
private:
    std::deque<Vector2Int> m_body;
    Vector2Int m_direction;
    Direction m_tempDirection;
    Grid& m_grid;
    bool m_isAlive {true};
    
public:
    
    Snake(Vector2Int startPosition, Grid& grid);
    ~Snake();
    
    // Get the first position
    Vector2Int Head() const;
    
    // in what direction the snake is moving 
    Direction Dir() const;
    Vector2Int Dir(Direction d) ;
    
    // Changes the direction the snake is moving
    void SetNextDirection(const Direction d);
    
    // Move the snake
    void Move(const bool grow);
    
    // Check if the snake is ocupining grid cell
    bool Occupies(const Vector2Int& pos) const;
    
    // if the snake hits itself
    bool HitSelf() const;
    
    bool IsAlive() const;
    
};
