#include "Snake.h"


Snake::Snake(Vector2Int startPosition, Grid& grid) :
    m_grid(grid)
{
    m_body.push_back(startPosition);
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
Vector2Int Snake::Dir(Direction d) const
{
    switch (d)
    {
        case Direction::North:      return Vector2Int(0,-1);
        case Direction::South:      return Vector2Int(0,1);
        case Direction::East:       return Vector2Int(1,0);
        case Direction::West:       return Vector2Int(-1,0);
        case Direction::Nothing:    return Vector2Int(0,0);
    }
    return Vector2Int(0,0);
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

void Snake::Move(const bool grow)
{
    Vector2Int lastBodyPos = Head();
    
    for (int i = 0; i < m_body.size(); ++i)
    {
        Vector2Int oldBodyPos { m_body[i]};
        Vector2Int newBodyPos;
        
        if ( i != 0 )
            newBodyPos = lastBodyPos;
        else
        {
            newBodyPos = m_body[0] + Dir(m_tempDirection);
            if (HitSelf())
                m_isAlive = false;
        }
        
        m_grid.SetCell(newBodyPos, CellType::Player);
        m_body[i] = newBodyPos;
        lastBodyPos = oldBodyPos;
    }
        
    m_grid.SetCell(lastBodyPos, CellType::Empty);
    if (grow)
        m_body.push_back(lastBodyPos);
    
    // m_grid.SetCell(m_body.front(), CellType::Player);
    if (Dir(m_tempDirection) != m_direction)
        m_direction = Dir(m_tempDirection);
}

bool Snake::Occupies(const Vector2Int& pos) const
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