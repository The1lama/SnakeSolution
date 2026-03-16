#include "AiInput.h"

#include <conio.h>

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

// AI Algorithms

void AiInput::RandomDirection(GameInfo& gameInfo)
{
    Snake& snake = gameInfo.snake;
    // Random direction every frame
    switch (rand() % 3)
    {
    case 0:    // [A] west
        if (snake.Dir() != Direction::East) // if the direction is not the opposite side of the moving direction
            snake.SetNextDirection(Direction::West); 
        break;
    case 1:   // [W] north
        if (snake.Dir() != Direction::South) // if the direction is not the opposite side of the moving direction
            snake.SetNextDirection(Direction::North); 
        break;
    case 2:   // [D] east
        if (snake.Dir() != Direction::West) // if the direction is not the opposite side of the moving direction
            snake.SetNextDirection(Direction::East); 
        break;
    case 3:   // [S] south
        if (snake.Dir() != Direction::North) // if the direction is not the opposite side of the moving direction
            snake.SetNextDirection(Direction::South); 
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
    
    // Get the distance to the food cell
    int distanceToFood = abs((foodPosition.x - snakePosition.x) + (foodPosition.y - snakePosition.y));

    Direction bestDir {};
    int bestDirVector = distanceToFood+99;
    
    for (int i = 0; i < 4; ++i)
    {
        Direction checkDirection = static_cast<Direction>(i);
        Vector2Int nextPosition = snake.Dir(checkDirection);
        CellType nextCell = grid.GetCell(snakePosition + nextPosition);
        
        // check if the next position in this direction is a wall or player
        // if it is, then jump to next direction
        if (nextCell == CellType::Wall || nextCell == CellType::Player)
            continue;
        
        if (checkDirection == GetOpisiteDirection(snake.Dir()))
            continue;
        
        Vector2Int offsetPosition = snake.GetPosition() + nextPosition;
        
        // check the distance to the food cell 
        // if the distance is smaller than the current best direction then change
        // Vector2Int checkDistance = (snakePosition + nextPosition) - foodPosition;
        int checkDistance = abs((foodPosition.x - offsetPosition.x) + (foodPosition.y - offsetPosition.y));
        if (checkDistance < bestDirVector)
        {
            bestDirVector = checkDistance;
            bestDir = checkDirection;
        }
    }
    
    // if the best Dir Vector IS NOT (99,99) then change the direction to the best 
    // if the best dir vector IS (99,99) then it means that the snake has boxed itself
        snake.SetNextDirection(bestDir);
    
}


Direction AiInput::GetOpisiteDirection(Direction other)
{
    switch (other)
    {
        case Direction::East:
            return Direction::West;
        case Direction::North:
            return Direction::South;
        case Direction::South:
            return Direction::North;
        case Direction::West:
            return Direction::East;
    }
}


////// PUBLIC //////

void AiInput::GetNextInput(GameInfo& gameInfo)
{
    // if user wants to quit game
    QuitGameInput(gameInfo);
    
    GreedyInput(gameInfo);
    // RandomDirection(gameInfo);
}
