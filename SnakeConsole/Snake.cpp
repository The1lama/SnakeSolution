#include "Snake.h"


Snake::Snake(Vector2Int startPosition, char charType) :
BaseEntity(charType)
{
    m_body.push_back(startPosition);
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
Vector2Int Snake::GetPosition() const
{
    return Head();
}

void Snake::RenderToGrid(Grid& grid) const
{
    for (auto bodyPart: m_body)
    {
        grid.SetCell(bodyPart, CellType::Player);
    }
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

void Snake::Move(Grid& grid)
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
        
        m_body[i] = newBodyPos;
        lastBodyPos = oldBodyPos;
    }
        
    grid.SetCell(lastBodyPos, CellType::Empty);
    if (m_grow)
    {
        Grow();
        m_body.push_back(lastBodyPos);
    }
    
    if (Dir(m_tempDirection) != m_direction)
        m_direction = Dir(m_tempDirection);
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
