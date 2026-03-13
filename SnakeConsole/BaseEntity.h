#pragma once
#include <random>

#include "Grid.h"
#include "Vector2Int.h"

class BaseEntity
{
protected:
    char m_charType;
    
    int static GetRandomNumber(int max)
    {
        // Obtains a random number from hardware to seed 
        std::random_device rd;
        // Initializes random generator with seed
        std::mt19937 gen(rd());
        
        int maxRange = static_cast<int>(max);
        std::uniform_int_distribution<int> dist(0, maxRange - 1);
        return dist(gen);
    }
    
public:
    BaseEntity(char charType);
    virtual ~BaseEntity() = default;
    
    virtual void RenderToGrid(Grid& grid) const = 0;
    
    // Gets the character char type
    virtual char GetCharType() const = 0;
    // Set the character char type
    virtual void SetCharType(char charType) = 0;
    
    virtual Vector2Int GetPosition() const = 0;
    virtual bool Occupies(Vector2Int pos) const = 0;
};
