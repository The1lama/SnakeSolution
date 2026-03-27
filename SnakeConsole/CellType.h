#pragma once
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <optional>

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
    Cell(CellType cellType, Vector2Int pos) : 
        type(cellType),
        position(pos)
    {}
    ~Cell() = default;
    
    CellType type;
    /**
     * This position can only have on position AND cannot be changed
     */
    Vector2Int position;
    bool isWalkable() const
    {
        return !(type == CellType::Wall || type == CellType::Player);
    }

    
    
    std::optional<Vector2Int> parentPosition;
    
    bool visited{false};
    
    int g{9999};    // the cost from the start node 
    int h{0};       // the heuristic cost to the end node
    
    // sum of for the cost of g and h
    int GetF() const
    {
        if (g == INT_MAX) return INT_MAX;
        return g+h;
    }
    
    int GetHeuristicCost(const Vector2Int& goalCell) const
    {
        return abs(position.x - goalCell.x) + abs(position.y - goalCell.y);
    }
    
    void ResetCellValue()
    {
        parentPosition.reset();
        g = 9999;
        h=0;
        visited = false;
    }
    
    bool operator== (const Cell& other) const
    {
        return position == other.position;
    }
};