#include "GameInfo.h"

GameInfo::GameInfo(bool& _running, Grid& _grid, Snake& _snake, FoodEntity& _food): 
    running(_running),
    grid(_grid),
    snake(_snake),
    food(_food)
{}
