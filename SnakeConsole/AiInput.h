#pragma once
#include "InputSystem.h"

class AiInput : public InputSystem
{
private:
    std::deque<CardinalValues> pathCells;
    
    
    void RandomDirection(GameInfo& gameInfo);
    void QuitGameInput(GameInfo& gameInfo);
    void GreedyInput(GameInfo& gameInfo);
    void AStarInput(GameInfo& gameInfo);
    
    void ReconstructInputPath(const Cell& startPos, const Cell& endPos, Grid& grid);
    int GetHeuristicCost(const Vector2Int& startCell, const Vector2Int& goalCell) const;
    Cell* GetBestCell(const std::vector<Cell*>& openSet) const;
    
public:
    void GetNextInput(GameInfo& gameInfo) override;
    
};
