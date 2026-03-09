#include "Snake.h"


Snake::Snake(Vector2Int startPosition, Grid& grid) :
    m_grid(grid)
{
    m_body.push_front(startPosition);
}

Snake::~Snake()
{
}


Vector2Int Snake::Head() const
{
    return m_body.front();
}

Direction Snake::Dir() const
{
    if(m_direction == Vector2Int(0,-1))  return Direction::North;
    if(m_direction == Vector2Int(0,1))  return Direction::South;
    if(m_direction == Vector2Int(1,0))  return Direction::East;
    if(m_direction == Vector2Int(-1,0))  return Direction::West;
    return Direction::Nothing;
}
Vector2Int Snake::Dir(Direction d) 
{
    switch (d)
    {
        case Direction::North:      return Vector2Int(0,-1);
        case Direction::South:      return Vector2Int(0,1);
        case Direction::East:       return Vector2Int(1,0);
        case Direction::West:       return Vector2Int(-1,0);
        case Direction::Nothing:    return Vector2Int(0,0);
    }
}

void Snake::SetNextDirection(const Direction d)
{
    m_tempDirection = d;
}

void Snake::Move(const bool grow)
{
    Vector2Int frontPosition = Head();
    Vector2Int directionOffset = Dir(m_tempDirection);
    
    if (!m_grid.InBounds(frontPosition+directionOffset))
        m_isAlive = false;
    
    if (grow)
        m_body.push_front(Vector2Int(frontPosition.x + directionOffset.x, frontPosition.y + directionOffset.y));
    else
    {
        m_grid.SetCell(m_body.back(), CellType::Empty);
        m_body.pop_back();        
        m_body.push_front(Vector2Int(frontPosition.x + directionOffset.x, frontPosition.y + directionOffset.y));
    }
    
    m_grid.SetCell(m_body.front(), CellType::Player);
    if (Dir(m_tempDirection) != m_direction)
        m_direction = Dir(m_tempDirection);
}

bool Snake::Occupies(const Vector2Int& pos) const
{
    return std::ranges::find(m_body, pos) != m_body.end();
}

bool Snake::HitSelf() const
{
    return Occupies(m_body.front()+m_direction);
}

bool Snake::IsAlive() const
{
    return m_isAlive;
}