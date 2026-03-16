#pragma once

struct Vector2Int
{
    int x {};
    int y {};
    
    bool operator== (const Vector2Int& other) const
    {
        return x == other.x && y == other.y;
    }
    bool operator!= (const Vector2Int& other) const
    {
        return x != other.x || y != other.y;
    }
    Vector2Int operator+ (const Vector2Int& other) const
    {
        return Vector2Int{x + other.x, y + other.y};
    }
    Vector2Int operator- (const Vector2Int& other) const
    {
        return Vector2Int{x - other.x, y - other.y};
    }

    bool operator<(const Vector2Int& other) const
    {
        if (x < other.x ) return true;
        if (x > other.x ) return false;
        return y < other.y;
    }
    bool operator>(const Vector2Int& other) const
    {
        if (x > other.x ) return true;
        if (x < other.x ) return false;
        return y > other.y;
    }
};
