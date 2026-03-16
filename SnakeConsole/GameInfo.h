#pragma once
#include "Grid.h"
#include "Snake.h"

class FoodEntity;

struct GameInfo
{
    bool& running;
    Grid& grid;
    Snake& snake;
    FoodEntity& food;
    
    GameInfo(bool& _running, Grid& _grid, Snake& _snake, FoodEntity& _food);
};

