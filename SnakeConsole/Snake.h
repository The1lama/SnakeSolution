#pragma once
#include <deque>

#include "BaseEntity.h"
#include "Direction.h"
#include "Grid.h"
#include "Vector2Int.h"

class Snake : public BaseEntity
{
private:
    std::deque<Vector2Int> m_body;
    Direction m_direction {Direction::East};
    Direction m_tempDirection {Direction::East};
    bool m_isAlive {true};
    bool m_grow {false};
    
public:
    Snake(Vector2Int startPosition, char charType);
    // Gets the character char type
    char GetCharType() const override;
    // Set the character char type
    void SetCharType(char charType) override;
    
    void RenderToGrid(Grid& grid) const override;
    
    // Get the first position
    Vector2Int GetPosition() const override;
    // Get the first position
    Vector2Int Head() const;
    
    // in what direction the snake is moving 
    Direction Dir() const;
    Vector2Int Dir(Direction d) const;
    
    // Changes the direction the snake is moving
    void SetNextDirection(const Direction d);
    
    // Set the starting DIRECTION of the snake, Default if east
    void SetStartingDirection(const Direction d);
    
    // Move the snake
    void Move(Grid& grid);
    
    // Check if the snake is ocupining grid cell
    bool Occupies(const Vector2Int pos) const override;
    
    // if the snake hits itself
    bool HitSelf() const;
    
    bool IsAlive() const;
    
    void Grow() ;
    
};
