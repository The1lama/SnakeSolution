#pragma once
#include "BaseEntity.h"

class WallEntity : public BaseEntity
{
private:
    Vector2Int m_Position;
    
public:
    WallEntity(Vector2Int pos, char charType);
    
    void RenderToGrid(Grid& grid) const override;
    
    // Gets the character char type
    char GetCharType() const override;
    // Set the character char type
    void SetCharType(char charType) override;
    
    Vector2Int GetPosition() const override;
    
    bool Occupies(Vector2Int pos) const override;
    
};
