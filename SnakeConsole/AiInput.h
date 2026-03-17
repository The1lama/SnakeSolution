#pragma once
#include "InputSystem.h"

class AiInput : public InputSystem
{
private:
    void RandomDirection(GameInfo& gameInfo);
    void QuitGameInput(GameInfo& gameInfo);
    void GreedyInput(GameInfo& gameInfo);
public:
    void GetNextInput(GameInfo& gameInfo) override;
    
};
