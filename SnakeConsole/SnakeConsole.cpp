#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <conio.h>
#include <fstream>
#include <string>
#include <Windows.h>
#include <limits>

#include  "Vector2Int.h"
#include "CellType.h"
#include "Grid.h"
#include "Food.h"
#include "Snake.h"
#include "SaveFile.h"
#include "SaveData.h"


#pragma region Defines

#define CLEAR_SCREEN std::cout << "\x1b[2J\x1b[H";

#pragma endregion Defines

#pragma region Functions

void SpawnMainMenu();

#pragma endregion Functions

namespace Helper
{
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

void ShowCursor(const bool show)
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 100;
    cursor.bVisible = show;
    SetConsoleCursorInfo(consoleHandle, &cursor);
}

void ClearSnakeScreen()
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{0,0});
}
    
}


enum class GameState
{
    MainMenu,
    PlaySnake,
    Help,
    Highscore,
    Quit,
};

struct GameSettings
{
    const int gridWidth {20};
    const int gridHeight {10};
    const std::string savePath { "SaveFile.txt" };
};

GameSettings gameSettings;

// spawns food at a random grid position
void SpawnFood(Grid& grid,const Snake& snake, Food& food_position)
{
    grid.SetCell(food_position.position, CellType::Empty);
    while (true)
    {
        // Gets a random pos in the grid_area
        int randX {Helper::GetRandomNumber(grid.Width()) - 1  };
        int randY {Helper::GetRandomNumber(grid.Height()) - 1 };
        
        if (!grid.InBounds(randX, randY), snake.Occupies(Vector2Int(randX,randY)))
            continue;
        
        Vector2Int newFoodPosition = Vector2Int{randX,randY};
        
        // if the food spawns inside a wall or player it finds a new random place to spawn
        CellType vectorValue { grid.GetCell(newFoodPosition)};
        if (!(vectorValue == CellType::Wall || vectorValue == CellType::Player))
        {
            food_position.position = newFoodPosition;
            grid.SetCell(newFoodPosition, CellType::Food);
            break;
        }
    }
}

// Gets player input when playing snake
void GetPlayerInput(bool& running, Snake& snake)
{
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
            running = false;
            break;
        default:
            break;
        }   
    }
}

void DrawGameOverScreen(int gameScore)
{
    // After game has ended
    CLEAR_SCREEN
    Helper::ShowCursor(TRUE);
    std::cout << 
        "=================\n" <<
        "    Game Over    \n" << 
        "=================\n";
    std::cout <<
        "You score: " << gameScore << "\n" <<
        "Type in you username/ Or have one character for it should not save\n"<<
        ">>> ";
    
    std::string name { };
    std::cin >> name;
    if (name.length() > 1)
    {
        std::cout << name;
        SaveFile::SaveHighscoreData(SaveData({name, gameScore}), gameSettings.savePath);
    }
}

void PlaySnake()
{
    CLEAR_SCREEN        // removes menu screen text 
    Helper::ShowCursor(FALSE);      // Hides cursor
    bool running { true };
    int gameScore {};
    
    // width and height of the grid
    int width { 20 };
    int height { 10 };
    
    
    Grid grid = Grid(width,height);
    Snake snake(Vector2Int(width/2,height/2), grid);
    
    Food foodPosition;
    
    // food position
    SpawnFood(grid, snake, foodPosition);
    
    grid.Render();
    // set timer for how often the snake updates
    auto lastUpdate = std::chrono::steady_clock::now();
    std::chrono::milliseconds interval {150};
    
    // Snake game loop
    while (running)
    {
        auto now = std::chrono::steady_clock::now();
        GetPlayerInput(running, snake);
        
        if (now - lastUpdate >= interval)
        {
            if (!snake.IsAlive() || !grid.InBounds(snake.Head()))
                break;
            
            bool growSnake {false};
            if (snake.Occupies(foodPosition.position))
            {
                growSnake = true;
                ++gameScore;
                SpawnFood(grid, snake, foodPosition);
            }
            
            snake.Move(growSnake);
            
            grid.Render();
            lastUpdate = now;
        }
    }
    
    DrawGameOverScreen(gameScore);
}

void DrawLeaderboard()
{
    CLEAR_SCREEN
    std::vector<SaveData> saveFile {SaveFile::GetHighscoreData(gameSettings.savePath)};
    
    std::cout << "===== High score =====\n\n";
    for (int i = 0; i < static_cast<int>(saveFile.size()); ++i)
    {
        std::cout <<
            i+1 << ". " << saveFile[i].playerName << ": " << saveFile[i].score << "\n"; 
        if (i>=4)   // only show top 5 scores
            break;
    }
    std::cout << "\n=== press ENTER to exit ===";
    
    while (!_kbhit()){}
}

void DrawHelpMenu()
{
    CLEAR_SCREEN
    std::cout << "===== Control/ Help Menu =====\n\n";
    std::cout << "Walls: #\n" << "Food: *\n" << "Player: @\n";
    std::cout << "W/A/S/D:\tUP/LEFT/DOWN/RIGHT\n";
    std::cout << "\n=== press ENTER to exit ===";
    // waits for user keyboard input
    while (!_kbhit()){}
}

void DrawMainMenu()
{
    CLEAR_SCREEN
    std::cout << "===== Welcome to Snake =====\n";
    std::cout << "  Press a number to choose what to do\n";
    std::cout << "  1) Play Snake \n" 
              << "  2) Controls/Help\n" 
              << "  3) Leaderboard\n"
              << "  4) Quit\n"
              << "============================\n";
   
}

int ReadIntInRange(int minVal, int maxVal)
{
    while (true)
    {
        std::cout << ">>> ";
        int choice {};
        std::cin >> choice;
        if (!std::cin.fail() && choice >= minVal && choice <= maxVal)
        {
            CLEAR_SCREEN
            return choice;
        }
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        std::cout << "Invalid input. Please try again.\n";
    }

}

void SpawnMainMenu()
{
    Helper::ShowCursor(TRUE);
    
    GameState currentState { GameState::MainMenu };
    
    while (currentState != GameState::Quit)
    {
        switch (currentState)
        {
        case GameState::MainMenu:
        {
            DrawMainMenu();
            int fd = ReadIntInRange(1,4);
            if (fd == 1) currentState = GameState::PlaySnake;
            if (fd == 2) currentState = GameState::Help;
            if (fd == 3) currentState = GameState::Highscore;
            if (fd == 4) currentState = GameState::Quit;
            break;
        }
        case GameState::PlaySnake:
        {
            PlaySnake();
            currentState = GameState::MainMenu;
            break;
        }
        case GameState::Help:
        {
            DrawHelpMenu();
            currentState = GameState::MainMenu;
            break;
        }
        case GameState::Highscore:
        {
            DrawLeaderboard();    
            currentState = GameState::MainMenu;
            break;
        }
        case GameState::Quit:
        {
            currentState = GameState::Quit;
            break;
        }
        }
    }
}

int main(int argc, char* argv[])
{
    SpawnMainMenu();
    return 0;
}
