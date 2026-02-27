#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <conio.h>
#include <Windows.h>

#pragma region Defines

#define CLEAR_SCREEN system("cls");
#define SizeToInt(x)  static_cast<int>(x)
#define twoDArray std::vector<std::vector<int>>

struct Vector2Int
{
    int x; int y;
    
    // method override function for equals to 
    bool operator==(const Vector2Int& other) const
    {
        return x == other.x && y == other.y;
    }
    // method override function for not equals
    bool operator!=(const Vector2Int& other) const
    {
        return !(*this == other);
    }
    // if putting a - before the Vector2Int it switches to minus
    Vector2Int operator-() const
    {
        return Vector2Int{-x, -y};
    }
    
    
};

#pragma endregion Defines

#pragma region Functions

void SpawnMainMenu();

#pragma endregion Functions

#pragma region Helpers

int static GetRandomNumber(int max)
{
    // Obtains a random number from hardware to seed 
    std::random_device rd;
    // Initializes random generator with seed
    std::mt19937 gen(rd());
    
    int maxRange = static_cast<int>(max);
    std::uniform_int_distribution<int> dist(0, maxRange - 1);
    return dist(gen);
}

void hideCursor(const bool show)
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 100;
    cursor.bVisible = show;
    SetConsoleCursorInfo(consoleHandle, &cursor);
}

#pragma endregion Helpers

#pragma region Grid

void RenderGrid(twoDArray& grid_area)
{
    CLEAR_SCREEN
    for (int y = 0; y < SizeToInt(grid_area[0].size()); y++)
    {
        for (int x = 0; x < SizeToInt(grid_area.size()); x++)
        {
            switch (grid_area[x][y])
            {
            case 1:     // is wall
                std::cout << '#';
                break;
            case 2:     // is food
                std::cout << '*';
                break;
            case 3:
                std::cout << '@';
                break;
            default:    // is white space
                std::cout << ' ';
            }
        }
        std::cout << '\n';
    }
}

bool IsInsideGrid(int x, int y, const twoDArray& gridArea)
{
    return x > 0 && x < SizeToInt(gridArea.size()-1)
        && y > 0 && y < SizeToInt(gridArea[0].size()-1);
}
int CheckGridValue(int x, int y, const twoDArray& gridArea)
{
    return gridArea[x][y];
}

twoDArray GenerateGrid(int width, int height)
{
    // creates a 2d dynamic array 
    std::vector<std::vector<int>> gridVectors;
    // sets the size of the 2d dynamic array
    gridVectors.resize(width, std::vector<int>(height));
    
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (x == 0 || x == width -1 || y == 0 || y == height -1)
                gridVectors[x][y] = 1;
            else 
                gridVectors[x][y] = 0;
        }
    }
    
    return gridVectors;
}

#pragma endregion Grid

// spawns food at a random grid position
void SpawnFood(twoDArray& grid_area, std::vector<Vector2Int> snakeBody)
{
    // Gets a random pos in the grid_area
    int randX { GetRandomNumber(SizeToInt(grid_area.size())) - 1  };
    int randY { GetRandomNumber(SizeToInt(grid_area[0].size())) - 1 };
    
    // checks if the random pos is inside the grid_area
    // if not rerun this function until it has found a valid pos
    // else place the food at coordinate
    if (!IsInsideGrid(randX, randY, grid_area))
    {
        SpawnFood(grid_area, snakeBody);
        return;
    }
    int vectorValue { CheckGridValue(randX, randY, grid_area) };
    if (vectorValue == 1 || vectorValue == 3)
    {
        SpawnFood(grid_area, snakeBody);
        return;
    }
    grid_area[randX][randY] = 2;
}

void PlaySnake()
{
    hideCursor(FALSE);
    bool running { true };
    
    // width and height of the grid
    int width { 20 };
    int height { 10 };
    
    // snake movement
    Vector2Int direction { 1,0 };
    // only for snake head
    Vector2Int snakeHeadPosition {width/2,height/2};
    std::vector<Vector2Int> snakeBody {snakeHeadPosition};
    
    twoDArray gridArea {GenerateGrid(width,height) };
    SpawnFood(gridArea, snakeBody);
    RenderGrid(gridArea);
    
    // set timer for how often the snake updates
    auto lastUpdate = std::chrono::steady_clock::now();
    std::chrono::milliseconds interval {250};
    
    
    while (running)
    {
        auto now = std::chrono::steady_clock::now();
        if ( _kbhit() )
        {
            switch (_getch())
            {
            case 97:    // [A] west
                if (direction != Vector2Int{1,0}) // if the direction is not the opposite side of the moving direction
                    direction = {-1,0};
                break;
            case 119:   // [W] north
                if (direction != Vector2Int{0,-1}) // if the direction is not the opposite side of the moving direction
                    direction = {0,-1};
                break;
            case 100:   // [D] east
                if (direction != Vector2Int{-1,0}) // if the direction is not the opposite side of the moving direction
                    direction = {1,0};
                break;
            case 115:   // [S] south
                if (direction != Vector2Int{0,-1}) // if the direction is not the opposite side of the moving direction
                    direction = {0,1};
                break;
            case 113:   // [Q] To main menu
                running = false;
                break;
            default:
                break;
            }   
        }
        
        if (now - lastUpdate >= interval)
        {
            // for initializing the snake start body position, Start from snake head
            Vector2Int lastBodyPosition {snakeHeadPosition};
            bool SnakeGrowing { false };
            // for moving the whole snake
            for (int i = 0; i < SizeToInt(snakeBody.size()); ++i)
            {
                Vector2Int snakeBodyPart { snakeBody[i] };
                Vector2Int newSnakePosition;
                // check if snake part is body or head
                if ( i != 0 )
                {
                    // move by lastBodyPosition
                    newSnakePosition = { lastBodyPosition.x, lastBodyPosition.y };
                }
                else
                {
                    // Move the snake head by direction
                    newSnakePosition = { snakeHeadPosition.x + direction.x, snakeHeadPosition.y + direction.y };
                    
                    // Check the next position the snake is going to hit,
                    //if the snake hits wall(1) or a body part of the snake(3), then break the game loop
                    int nextGridValue { CheckGridValue(newSnakePosition.x, newSnakePosition.y, gridArea) };
                    if (nextGridValue == 1 || (nextGridValue == 3))
                    {
                        running = false;
                        break;
                    }
                    //else if snake hits food(2), make the snake longer
                    else if (nextGridValue == 2)
                        SnakeGrowing = true;
                    
                    snakeHeadPosition = newSnakePosition;
                }
                // updates snake body part position
                snakeBody[i] = newSnakePosition;
                // Removes old snake part (for Drawing)
                gridArea[snakeBodyPart.x][snakeBodyPart.y] = 0;
                // Draws new snake part at new position 
                gridArea[newSnakePosition.x][newSnakePosition.y] = 3;
                // sets the current body position so the next position can read where it should go
                lastBodyPosition = snakeBodyPart;
            }
            if (SnakeGrowing)
            {
                // creates a new snake body part and 
                // spawn in a new food at random position
                snakeBody.push_back(lastBodyPosition);
                SpawnFood(gridArea, snakeBody);
            }
            lastUpdate = now;
            RenderGrid(gridArea);
        }
    }
    
    // After game has ended
    CLEAR_SCREEN
    std::cout << "Ending Snake game";
    while (!_kbhit()){}
    SpawnMainMenu();
    CLEAR_SCREEN
}

void DrawHelpMenu()
{
    CLEAR_SCREEN
    std::cout << "===== Control/ Help Menu =====\n";
    std::cout << "Walls: #\n" << "Food: *\n" << "Player: @\n";
    std::cout << "W/A/S/D:\tUP/LEFT/DOWN/RIGHT\n";
    std::cout << "\n=== press button to exit ===";
    // waits for user keyboard input
    while (!_kbhit()){}
    
}

void SpawnMainMenu()
{
    hideCursor(TRUE);
    bool running { true };
    
    while (running)
    {
        CLEAR_SCREEN
        std::cout << "===== Welcome to Snake =====\n";
        std::cout << "Press a number to choose what to do\n";
        std::cout << "1) Play Snake \n" << "2) Controls/Help\n" << "3) Quit\n";
        std::cout << "> ";
        
        int choice;
        std::cin >> choice;
        if (std::cin.fail())
        {
            CLEAR_SCREEN
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid input. Please try again.\n";
            while (!_kbhit()){}
        }
        
        switch (choice)
        {
        case 1:
            PlaySnake();
            break;
        case 2:
            DrawHelpMenu();
            break;
        case 3:
            running = false;
            break;
        default:
            CLEAR_SCREEN
            std::cout << "Invalid input. Please try again.\n";
            while (!_kbhit()){}
        }
    }
}

int main(int argc, char* argv[])
{
    SpawnMainMenu();
    return 0;
}
