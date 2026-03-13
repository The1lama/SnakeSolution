#include <array>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <conio.h>
#include <string>
#include <Windows.h>
#include <limits>

#include "BaseEntity.h"
#include "Vector2Int.h"
#include "CellType.h"
#include "Grid.h"
#include "Food.h"
#include "FoodEntity.h"
#include "Snake.h"
#include "SaveFile.h"
#include "SaveData.h"
#include "GameSettings.h"
#include "WallEntity.h"

#define CLEAR_SCREEN std::cout << "\x1b[2J\x1b[H";


namespace Helper
{


    static void ShowCursor(const bool show)
    {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursor;
        cursor.dwSize = 100;
        cursor.bVisible = show;
        SetConsoleCursorInfo(consoleHandle, &cursor);
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



GameSettings gameSettings;



// Gets player input when playing snake, will skip if no input is given during the frame
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
    
    GameSettings gameSettings {};
    
    Grid grid = Grid(gameSettings.width,gameSettings.height);
    grid.SetEmptyChar(gameSettings.emptyChar);
    grid.SetFoodChar(gameSettings.foodChar);
    grid.SetPlayerChar(gameSettings.playerChar);
    grid.SetWallChar(gameSettings.wallChar);
    
    Snake snake(Vector2Int{gameSettings.width/2,gameSettings.height/2}, '@');
    
    FoodEntity foodEntity{Vector2Int{0,0}, gameSettings.foodChar};
    
    // food position
    foodEntity.SpawnFood(grid, snake);
    
    // set timer for how often the snake updates
    auto lastUpdate = std::chrono::steady_clock::now();
    std::chrono::milliseconds interval {150};
    
    grid.Render();
    // Snake game loop
    while (running)
    {
        auto now = std::chrono::steady_clock::now();
        GetPlayerInput(running, snake);
        
        if (now - lastUpdate >= interval)
        {
            if (!snake.IsAlive() || !grid.InBounds(snake.Head()))
                break;
            
            snake.Move(grid);
            if (snake.Occupies(foodEntity.GetPosition()))
            {
                snake.Grow();
                ++gameScore;
                foodEntity.SpawnFood(grid, snake);
            }
            
            snake.RenderToGrid(grid);
            foodEntity.RenderToGrid(grid);
            
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

void WarmUp()
{
    std::vector<std::unique_ptr<BaseEntity>>  entities {};
    
    entities.push_back(std::make_unique<FoodEntity>(Vector2Int{1,2}, '*'));
    entities.push_back(std::make_unique<WallEntity>(Vector2Int{3,4}, '#'));
    
    for (auto& entity : entities)
    {
        auto isOcupied = entity -> Occupies(Vector2Int{1,2});
        std::cout << (isOcupied ? "Occupied" : "Free") << std::endl;
    }
    
}

int main(int argc, char* argv[])
{
    SpawnMainMenu();
    // WarmUp();
    
    std::cin >> std::ws;
    return 0;
}
