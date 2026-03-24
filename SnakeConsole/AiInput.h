#pragma once
#include "InputSystem.h"

class AiInput : public InputSystem
{
private:
    void RandomDirection(GameInfo& gameInfo);
    void QuitGameInput(GameInfo& gameInfo);
    void GreedyInput(GameInfo& gameInfo);
    void AStarInput(GameInfo& gameInfo);
    
    int GetHeuristicCost(const Vector2Int& startCell, const Vector2Int& goalCell) const;
    
public:
    void GetNextInput(GameInfo& gameInfo) override;
    
};
