#pragma once
#include "InputSystem.h"

class PlayerInput : public InputSystem
{
public:
    void GetNextInput(GameInfo& gameInfo) override;
};
