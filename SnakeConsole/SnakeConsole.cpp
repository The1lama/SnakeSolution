#include <chrono>
#include <conio.h>

#include "BaseEntity.h"
#include "Vector2Int.h"
#include "Grid.h"
#include "FoodEntity.h"
#include "Snake.h"
#include "SaveFile.h"
#include "SaveData.h"
#include "GameSettings.h"
#include "InputSystem.h"
#include "PlayerInput.h"
#include "AiInput.h"
#include "GameInfo.h"

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


enum class SnakeGameState
{
    MainMenu,
    PlaySnake,
    AiSnake,
    Help,
    Highscore,
    Quit,
};



GameSettings gameSettings;

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


void PlaySnake(bool aiPlay = false)
{
    CLEAR_SCREEN        // removes menu screen text 
    Helper::ShowCursor(FALSE);      // Hides cursor
    bool running { true };
    int gameScore {};
    
    
    Grid grid = Grid(gameSettings.width,gameSettings.height);
    grid.SetEmptyChar(gameSettings.emptyChar);
    grid.SetFoodChar(gameSettings.foodChar);
    grid.SetPlayerChar(gameSettings.playerChar);
    grid.SetWallChar(gameSettings.wallChar);
    
    Snake snake(Vector2Int{gameSettings.width/2,gameSettings.height/2}, '@');
    
    FoodEntity foodEntity{Vector2Int{1,1}, gameSettings.foodChar};
    
    GameInfo gameInfo{running, grid, snake, foodEntity};
    
    
    // chooses the input system that the game should be using 
    // first if the player input 
    // second is the AI input
    std::unique_ptr<InputSystem> input_system;
    if (!aiPlay)
        input_system = std::make_unique<PlayerInput>();
    else
        input_system = std::make_unique<AiInput>();
    
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
        input_system->GetNextInput(gameInfo);
        
        if (now - lastUpdate >= interval)
        {
            snake.Move(grid);
            
            // if the snake has hit himself or 
            // that the snake head position is inside a wall 
            // GAME OVER, else render new grid
            if (!snake.IsAlive() || !grid.InBounds(snake.Head()))
            {
                running = false;
                break;
            }
            
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
              << "  4) AI Play\n"
              << "  5) Quit\n"
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
    
    SnakeGameState currentState { SnakeGameState::MainMenu };
    
    while (currentState != SnakeGameState::Quit)
    {
        switch (currentState)
        {
        case SnakeGameState::MainMenu:
        {
            DrawMainMenu();
            int fd = ReadIntInRange(1,5);
            if (fd == 1) currentState = SnakeGameState::PlaySnake;
            if (fd == 2) currentState = SnakeGameState::Help;
            if (fd == 3) currentState = SnakeGameState::Highscore;
            if (fd == 4) currentState = SnakeGameState::AiSnake;
            if (fd == 5) currentState = SnakeGameState::Quit;
            break;
        }
        case SnakeGameState::PlaySnake:
        {
            PlaySnake();
            currentState = SnakeGameState::MainMenu;
            break;
        }
        case SnakeGameState::AiSnake:
        {
            PlaySnake(true);
            currentState = SnakeGameState::MainMenu;
            break;
        }
        case SnakeGameState::Help:
        {
            DrawHelpMenu();
            currentState = SnakeGameState::MainMenu;
            break;
        }
        case SnakeGameState::Highscore:
        {
            DrawLeaderboard();    
            currentState = SnakeGameState::MainMenu;
            break;
        }
        case SnakeGameState::Quit:
        {
            currentState = SnakeGameState::Quit;
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
