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

Direction Snake::Dir() const
{
    return m_direction;
}
Vector2Int Snake::Dir(Direction d) const
{
    switch (d)
    {
        case Direction::North:      return Vector2Int{0,-1};
        case Direction::South:      return Vector2Int{0,1};
        case Direction::East:       return Vector2Int{1,0};
        case Direction::West:       return Vector2Int{-1,0};
    }
    return Vector2Int{0,0};
}

Direction Snake::GetOpisiteDirection(Direction other) const
{
    switch (other)
    {
    case Direction::East:
        return Direction::West;
    case Direction::North:
        return Direction::South;
    case Direction::South:
        return Direction::North;
    case Direction::West:
        return Direction::East;
    }
    return Direction::North;
}
void Snake::SetNextDirection(const Direction d)
{
    m_tempDirection = d;
}

// Setting the starting direction of the Snake, Default is east
void Snake::SetStartingDirection(const Direction d)
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
