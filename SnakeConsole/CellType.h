#pragma once
#include <cstdint>

#include "Vector2Int.h"

// using the uint8_t to use less memory
enum class CellType : uint8_t
{
    Empty = 0,
    Wall = 1,
    Food = 2,
    Player = 3,
};

struct Cell
{
    Cell(CellType type) : 
        type(type){};
    ~Cell() = default;
    
    CellType type;
    bool isWalkable {!(type == CellType::Wall || type == CellType::Player)};
    
    Vector2Int parentPosition;
    int g;  // the cost from the start node 
    int h;  // the heuristic cost to the end node
    int f {g+h};    // sum of for the cost of g and h
};