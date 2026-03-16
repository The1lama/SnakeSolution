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
            if (snake.Dir() != Direction::East) // if the direction is not the opposite side of the moving direction
                snake.SetNextDirection(Direction::West); 
            break;
        case 119:   // [W] north
            if (snake.Dir() != Direction::South) // if the direction is not the opposite side of the moving direction
                snake.SetNextDirection(Direction::North); 
            break;
        case 100:   // [D] east
            if (snake.Dir() != Direction::West) // if the direction is not the opposite side of the moving direction
                snake.SetNextDirection(Direction::East); 
            break;
        case 115:   // [S] south
            if (snake.Dir() != Direction::North) // if the direction is not the opposite side of the moving direction
                snake.SetNextDirection(Direction::South); 
            break;
        case 113:   // [Q] To main menu
            gameInfo.running = false;
            break;
        default:
            break;
        }   
    }
}
