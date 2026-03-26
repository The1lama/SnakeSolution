#include "Snake.h"


Snake::Snake(Vector2Int startPosition, char charType) :
BaseEntity(charType)
{
    m_body.push_front(startPosition);
}

// Gets the character char type
char Snake::GetCharType() const
{
    return m_charType;
}
// Sets the character char type
void Snake::SetCharType(char charType)
{
    m_charType = charType;
}

void Snake::RenderToGrid(Grid& grid) const
{
    for (auto bodyPart: m_body)
    {
        grid.SetCell(bodyPart, CellType::Player);
    }
}

// Get first body position 
Vector2Int Snake::GetPosition() const
{
    return Head();
}
Vector2Int Snake::Head() const
{
    return m_body.front();
}

CardinalValues Snake::Dir() const
{
    return m_direction;
}
Vector2Int Snake::Dir(CardinalValues d) const
{
    switch (d)
    {
        case CardinalValues::North:      return Vector2Int{0,-1};
        case CardinalValues::South:      return Vector2Int{0,1};
        case CardinalValues::East:       return Vector2Int{1,0};
        case CardinalValues::West:       return Vector2Int{-1,0};
    }
    return Vector2Int{0,0};
}

CardinalValues Snake::GetOpisiteDirection(CardinalValues other) const
{
    switch (other)
    {
    case CardinalValues::East:
        return CardinalValues::West;
    case CardinalValues::North:
        return CardinalValues::South;
    case CardinalValues::South:
        return CardinalValues::North;
    case CardinalValues::West:
        return CardinalValues::East;
    }
    return CardinalValues::North;
}
void Snake::SetNextDirection(const CardinalValues d)
{
    m_tempDirection = d;
}

// Setting the starting direction of the Snake, Default is east
void Snake::SetStartingDirection(const CardinalValues d)
{
    m_tempDirection = d;
}

void Snake::Move(Grid& grid)
{
    // if the next move is that he hits him self, 
    // Game over
    if (HitSelf())
    {
        m_isAlive = false;
        return;
    }
    
    Vector2Int newHeadPosition = Head() + Dir(m_tempDirection);
    
    // check if the new head position is not on grid 
    // if it's not switch the position to the other side of the grid so the snake can loop
    if (!grid.InBounds(newHeadPosition))
    {
        if (Head().x == grid.Width() -1 || Head().x == 0)
        {
            newHeadPosition.x = Head().x == grid.Width() -1 ? 0 : grid.Width() - 1 ;
        }
        if (Head().y == grid.Height() -1 || Head().y == 0)
        {
            newHeadPosition.y = Head().y == grid.Height() -1? 0 : grid.Height() - 1 ;
        }
    }
    
    // Spawn in new head
    m_body.push_front(newHeadPosition);
    
    // if the snake has hit a food piece it should grow else,
    // remove the tail of the snake and make the cell empty
    if (m_grow)
        Grow();
    else
    {
        grid.SetCell(m_body.back(), CellType::Empty);
        m_body.pop_back();
    }
    
    // Set the temp direction to current direction
    if (m_tempDirection != m_direction)
        m_direction = m_tempDirection;
}

bool Snake::Occupies(const Vector2Int pos) const
{
    return std::ranges::find(m_body, pos) != m_body.end();
}

bool Snake::HitSelf() const
{
    return Occupies(m_body.front()+ Dir(m_tempDirection));
}

bool Snake::IsAlive() const
{
    return m_isAlive;
}

void Snake::Grow()
{
    m_grow = !m_grow;
}
