#pragma once
#include "InputSystem.h"

class AiInput : public InputSystem
{
private:
    void RandomDirection(GameInfo& gameInfo);
    void QuitGameInput(GameInfo& gameInfo);
    void GreedyInput(GameInfo& gameInfo);
    
    Direction GetOpisiteDirection(Direction otherDirection);
    
    template <class T>
    T abs(T value) const
    {
        return value * ((value > 0) - (value <0));
    }


public:
    void GetNextInput(GameInfo& gameInfo) override;
    
};
