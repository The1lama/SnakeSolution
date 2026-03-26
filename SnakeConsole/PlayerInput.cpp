#include "PlayerInput.h"

#include <conio.h>


void PlayerInput::GetNextInput(GameInfo& gameInfo)
{
    Snake& snake = gameInfo.snake;
    if ( _kbhit() )
    {
        switch (_getch())
        {
        case 97:    // [A] west
            if (snake.Dir() != CardinalValues::East) // if the direction is not the opposite side of the moving direction
                snake.SetNextDirection(CardinalValues::West); 
            break;
        case 119:   // [W] north
            if (snake.Dir() != CardinalValues::South) // if the direction is not the opposite side of the moving direction
                snake.SetNextDirection(CardinalValues::North); 
            break;
        case 100:   // [D] east
            if (snake.Dir() != CardinalValues::West) // if the direction is not the opposite side of the moving direction
                snake.SetNextDirection(CardinalValues::East); 
            break;
        case 115:   // [S] south
            if (snake.Dir() != CardinalValues::North) // if the direction is not the opposite side of the moving direction
                snake.SetNextDirection(CardinalValues::South); 
            break;
        case 113:   // [Q] To main menu
            gameInfo.running = false;
            break;
        default:
            break;
        }   
    }
}
