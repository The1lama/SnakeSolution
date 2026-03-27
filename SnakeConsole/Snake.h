#pragma once
#include <deque>

#include "BaseEntity.h"
#include "CardinalValues.h"
#include "Grid.h"
#include "Vector2Int.h"

class Snake : public BaseEntity
{
private:
    std::deque<Vector2Int> m_body;
    CardinalValues m_direction {CardinalValues::East};
    CardinalValues m_tempDirection {CardinalValues::East};
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
    std::deque<Vector2Int>& GetBodyRef();
    
    // in what direction the snake is moving 
    CardinalValues Dir() const;
    Vector2Int Dir(CardinalValues d) const;
    CardinalValues GetOpisiteDirection(CardinalValues other) const;
    
    // Changes the direction the snake is moving
    void SetNextDirection(const CardinalValues d);
    
    // Set the starting DIRECTION of the snake, Default if east
    void SetStartingDirection(const CardinalValues d);
    
    // Move the snake
    void Move(Grid& grid);
    
    // Check if the snake is ocupining grid cell
    bool Occupies(const Vector2Int pos) const override;
    
    // if the snake hits itself
    bool HitSelf() const;
    
    bool IsAlive() const;
    
    void Grow() ;
    
};
