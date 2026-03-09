#pragma once
#include <cstdint>

// using the uint8_t to use less memory
enum class CellType : uint8_t
{
    Empty = 0,
    Wall = 1,
    Food = 2,
    Player = 3,
};
