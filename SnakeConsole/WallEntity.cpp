#include "WallEntity.h"

#include <iostream>
#include <ostream>


WallEntity::WallEntity(Vector2Int pos, char charType) : 
BaseEntity(charType),
m_Position(pos)
{
}

char WallEntity::GetCharType() const
{
    return m_charType;
}
void WallEntity::SetCharType(char charType)
{
    m_charType = charType;
}

void WallEntity::RenderToGrid(Grid& grid) const
{
    grid.SetCell(m_Position, CellType::Wall);
}

Vector2Int WallEntity::GetPosition() const
{
    return m_Position;
}


bool WallEntity::Occupies(Vector2Int pos) const
{
    return m_Position == pos;
}
