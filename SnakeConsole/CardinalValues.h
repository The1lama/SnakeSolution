#pragma once
#include <cstdint>

#include "Vector2Int.h"

enum class CardinalValues : uint8_t
{
    North,  // UP
    East,   // RIGHT
    South,  // DOWN
    West,   // LEFT
};

struct Direction
{
    
    static Vector2Int GetDirectionByCardinal(CardinalValues direction)
    {
        switch (direction)
        {
        case CardinalValues::North:      return Vector2Int{0,-1};
        case CardinalValues::South:      return Vector2Int{0,1};
        case CardinalValues::East:       return Vector2Int{1,0};
        case CardinalValues::West:       return Vector2Int{-1,0};
        }
        return Vector2Int{0,0};
    }
    
    static CardinalValues GetDirectionByVector2Int(const Vector2Int& directionVector)
    {
        if (directionVector == Vector2Int{0,-1}) return CardinalValues::North;
        if (directionVector == Vector2Int{1,0}) return CardinalValues::East;
        if (directionVector == Vector2Int{-1,0}) return CardinalValues::West;
        if (directionVector == Vector2Int{0,1}) return CardinalValues::South;
        return CardinalValues::North;
    }
    
    static CardinalValues GetOpisiteDirection(CardinalValues other)
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
    
};