#pragma once

#include "GameInfo.h"
#include "Snake.h"

class InputSystem
{
public:
    virtual ~InputSystem() = default;
    
    
    virtual void GetNextInput(GameInfo& gameInfo) = 0;
    
};
