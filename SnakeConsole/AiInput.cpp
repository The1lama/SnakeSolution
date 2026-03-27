#include "AiInput.h"

#include <conio.h>

#include <algorithm>
#include <iostream>
#include <ostream>

#include "FoodEntity.h"

////// PRIVATE //////

void AiInput::QuitGameInput(GameInfo& gameInfo)
{
    if ( _kbhit() )
    {
        switch (_getch())
        {
        case 113:   // [Q] To main menu
            gameInfo.running = false;
            break;
        default:
            break;
        }   
    }
}

// AI Algorithms //

void AiInput::RandomDirection(GameInfo& gameInfo)
{
    Snake& snake = gameInfo.snake;
    // Random direction every frame
    switch (rand() % 3)
    {
    case 0:    // [A] west
        if (snake.Dir() != CardinalValues::East) // if the direction is not the opposite side of the moving direction
            snake.SetNextDirection(CardinalValues::West); 
        break;
    case 1:   // [W] north
        if (snake.Dir() != CardinalValues::South) // if the direction is not the opposite side of the moving direction
            snake.SetNextDirection(CardinalValues::North); 
        break;
    case 2:   // [D] east
        if (snake.Dir() != CardinalValues::West) // if the direction is not the opposite side of the moving direction
            snake.SetNextDirection(CardinalValues::East); 
        break;
    case 3:   // [S] south
        if (snake.Dir() != CardinalValues::North) // if the direction is not the opposite side of the moving direction
            snake.SetNextDirection(CardinalValues::South); 
        break;
    default: ;
    }
}

void AiInput::GreedyInput(GameInfo& gameInfo)
{
    Grid& grid = gameInfo.grid;
    FoodEntity& food = gameInfo.food;
    Snake& snake = gameInfo.snake;
    
    Vector2Int foodPosition = food.GetPosition();
    Vector2Int snakePosition = snake.GetPosition();

    CardinalValues bestDir {snake.Dir()};
    int bestDirVector = 99;
    
    for (int i = 0; i < 4; ++i)
    {
        CardinalValues checkDirection = static_cast<CardinalValues>(i);
        Vector2Int nextPosition = snake.Dir(checkDirection);
        CellType nextCell = grid.GetCell(snakePosition + nextPosition).type;
        
        // check if the next position in this direction is a wall or player
        // if it is, then jump to next direction
        // Also checks so that the snake cant walk into itself
        if (checkDirection == snake.GetOpisiteDirection(snake.Dir()) || 
            nextCell == CellType::Wall || nextCell == CellType::Player)
            continue;
        
        Vector2Int offsetPosition = snakePosition + nextPosition;
        
        // check the distance to the food cell 
        // if the distance is smaller than the current best direction then change
        int checkDistance = GetHeuristicCost(offsetPosition, foodPosition);
        if (checkDistance < bestDirVector)
        {
            bestDirVector = checkDistance;
            bestDir = checkDirection;
        }
    }
    
    snake.SetNextDirection(bestDir);
}

void AiInput::AStarInput(GameInfo& gameInfo)
{
    Vector2Int neighborDirection[4]
    {
        Vector2Int{0,-1},   // NORTH
        Vector2Int{1,0},    // EAST
        Vector2Int{-1,0},   // WEST
        Vector2Int{0,1}     // SOUTH
    };
    
    
    Grid& grid = gameInfo.grid;
    FoodEntity& food = gameInfo.food;
    Snake& snake = gameInfo.snake;
    
    // reset grid cell values
    grid.ResetGridCells();
    
    
    Vector2Int foodPosition = food.GetPosition();
    Vector2Int snakePosition = snake.GetPosition();
    
    Cell& startCell = grid.GetCellRef(snakePosition);
    startCell.g = 0;
    startCell.h = GetHeuristicCost(snakePosition, foodPosition);
    
    int rounds{0};
    
    // starts the algorithm from the snake head position
    std::vector<Cell*> openSet{&startCell};
    // The cells that we have visited
    
    while (!openSet.empty())
    {
        Cell* bestCell = GetBestCell(openSet);
        
        if (bestCell->position == foodPosition)
        {
            std::cout << "Path has been found" << std::endl;
            ReconstructInputPath(startCell, *bestCell, grid);
            return;
        }
        
        bestCell->visited = true;
        std::erase(openSet, bestCell);

        for (auto& neighborDir : neighborDirection)
        {
            Vector2Int neighborPos = bestCell->position + neighborDir;
            
            // if the cell is out of bounds
            if (!grid.InBounds(neighborPos))
                continue;
            
            Cell& neighborCell = grid.GetCellRef(neighborPos);
            
            // if the cell is a wall or player can not walk on it OR
            // the cell is already visided
            if (!neighborCell.isWalkable() || neighborCell.visited)
                continue;
            
            ++rounds;
            
            int tentativeGScore = bestCell->g + 1;
            
            // if the cost of the tentative g cost if higher than the neighbor it 
            // we dont need it 
            if (tentativeGScore > neighborCell.g) 
                continue;
            
            neighborCell.parentPosition = bestCell->position;
            neighborCell.g = tentativeGScore;
            neighborCell.h = neighborCell.GetHeuristicCost(foodPosition);
            
            // if the neighbor Cell does not contain in the openCells
            if (std::find(openSet.begin(), openSet.end(), &neighborCell) == openSet.end())
            {
                openSet.push_back(&neighborCell);
            }
            
        }
        
    }
    
    // if there is no path to the goal place
    // use the greedy algorithm as backup
    // GreedyInput(gameInfo);
}

void AiInput::ReconstructInputPath(const Cell& startPos, const Cell& endPos, Grid& grid)
{
    pathCells.clear();
    const Cell* current = &endPos;
    
    while (current->position != startPos.position)
    {
        if (!current->parentPosition.has_value()) break;
        
        Vector2Int parentPos = current->parentPosition.value();
        
        Vector2Int directionVector = current->position - parentPos;
        
        pathCells.push_back(Direction::GetDirectionByVector2Int(directionVector));
        current = &grid.GetCellRef(parentPos);
    }
    std::reverse(pathCells.begin(), pathCells.end());
}

// Get the Heuristic cost from the start cell to the goal cell
int AiInput::GetHeuristicCost(const Vector2Int& startCell, const Vector2Int& goalCell) const
{
    return abs(startCell.x - goalCell.x) + abs(startCell.y - goalCell.y);
}

Cell* AiInput::GetBestCell(const std::vector<Cell*>& openSet) const
{
    Cell* bestCell{openSet.front()};
    
    for (Cell* cell : openSet)
    {
        if (bestCell->GetF() > cell->GetF())
            bestCell = cell;
    }
    return bestCell;
}


////// PUBLIC //////

void AiInput::GetNextInput(GameInfo& gameInfo)
{
    // if user wants to quit game
    QuitGameInput(gameInfo);
    
    gameInfo.grid.SetCell(gameInfo.food.GetPosition(), CellType::Food);
    
    for (auto& body : gameInfo.snake.GetBodyRef())
    {
        gameInfo.grid.SetCell(body, CellType::Player);
    }
    
    
    ////// NOT WORKING ASTAR ALGORITHM //////
    if (pathCells.empty())
        AStarInput(gameInfo);
    
    if (!pathCells.empty())
    {
        CardinalValues direction{pathCells.front()};
        gameInfo.snake.SetNextDirection(direction);
        pathCells.pop_front();
    }
    /////////////////////////////////////////
    
    
    // GreedyInput(gameInfo);
    // RandomDirection(gameInfo);
}
