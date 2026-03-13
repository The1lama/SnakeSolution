#pragma once
#include "BaseEntity.h"
#include "Snake.h"

class FoodEntity : public BaseEntity
{
private:
    Vector2Int m_Position;
    
public:
    FoodEntity(Vector2Int pos, char charType);
    
    void RenderToGrid(Grid& grid) const override;
    
    void SpawnFood(Grid& grid, Snake& snake);
    
    // Gets the character char type
    char GetCharType() const override;
    // Set the character char type
    void SetCharType(char charType) override;
    
    void SpawnFood() const;
    
    Vector2Int GetPosition() const override;
    void SetPosition(Vector2Int pos);
    bool Occupies(Vector2Int pos) const;
};
