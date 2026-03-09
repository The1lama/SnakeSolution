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

namespace SaveFile
{
    const std::string fileName = "SaveFile.txt";
    
    struct SaveData
    {
        std::string name;
        int score;
    };
    
    void Sort(std::vector<SaveData>& data)
    {
        for (int i = 0; i < data.size()-1; ++i)
        {
            for (int j = 0; j < data.size()-i-1; ++j)
            {
                if (data[j].score < data[j+1].score)
                {
                    SaveData tempData = data[j];
                    data[j] = data[j+1];
                    data[j+1] = tempData;
                }
            }
        }
    }
    
    bool fileExists(const std::string& checkFileName)
    {
        return std::ifstream{checkFileName}.good();
    }
    
    void SaveHighscoreData(const SaveData data)
    {
        std::ofstream outf{ fileName, std::ios::app };
        if (!outf)
        {
            std::cerr << "Unable to open file \"" << fileName << "\"" << std::endl;
            return;
        }
        outf << data.score <<
            ' ' << data.name << '\n';
        outf.close();
    }
    
    std::vector<SaveData> GetHighscoreFile()
    {
        // if the file does not exist, creates a new file
        if (!fileExists(fileName))
        {
            std::ofstream outf{ "SaveFile.txt" };
            outf.close();
        }
        
        std::ifstream inf{ "SaveFile.txt" };
        
        // Could not open save file
        if (!inf)
        {
            std::cerr << "Unable to open file \"" << fileName << "\"" << std::endl;
            return std::vector<SaveData>{};
        }
        
        std::vector<SaveData> savedPlays;
        std::vector<std::string> strings;
        std::string strInput{};
        while (inf >> strInput)
        {
            strings.push_back(strInput);
            strInput.clear();
        }
        for (int i = 0; i < strings.size(); i+=2)
        {
            savedPlays.push_back(SaveData{strings[i+1], std::stoi(strings[i])});
        }

        Sort(savedPlays);
        return savedPlays;
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

// spawns food at a random grid position
void SpawnFood(Grid& grid, Food& food_position)
{
    while (true)
    {
        // Gets a random pos in the grid_area
        int randX {Helper::GetRandomNumber(grid.Width()) - 1  };
        int randY {Helper::GetRandomNumber(grid.Height()) - 1 };
        
        if (!grid.InBounds(randX, randY))
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
void GetPlayerInput(bool& running, Vector2Int directionValue, Vector2Int& temporaryDirectionValue)
{
    if ( _kbhit() )
    {
        switch (_getch())
        {
        case 97:    // [A] west
            if (directionValue != Vector2Int{1,0}) // if the direction is not the opposite side of the moving direction
                temporaryDirectionValue = {-1,0};
            break;
        case 119:   // [W] north
            if (directionValue != Vector2Int{0,-1}) // if the direction is not the opposite side of the moving direction
                temporaryDirectionValue = {0,-1};
            break;
        case 100:   // [D] east
            if (directionValue != Vector2Int{-1,0}) // if the direction is not the opposite side of the moving direction
                temporaryDirectionValue= {1,0};
            break;
        case 115:   // [S] south
            if (directionValue != Vector2Int{0,-1}) // if the direction is not the opposite side of the moving direction
                temporaryDirectionValue = {0,1};
            break;
        case 113:   // [Q] To main menu
            running = false;
            break;
        default:
            break;
        }   
    }
}

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



// Updates the snake position on grid 
void UpdateSnakePosition(bool& running, std::vector<Vector2Int>& snakeBody, 
    Grid& grid, Vector2Int& temporaryDirectionValue, int& gameScore)
{
    // for initializing the snake start body position, Start from snake head
    Vector2Int lastBodyPosition = snakeBody[0];
    bool SnakeGrowing { false };
    // for moving the whole snake
    for (int i = 0; i < static_cast<int>(snakeBody.size()); ++i)
    {
        Vector2Int oldSnakeBodyPartPosition { snakeBody[i] };
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
            newSnakePosition = { snakeBody[0].x + temporaryDirectionValue.x, snakeBody[0].y + temporaryDirectionValue.y };
                    
            // Check the next position the snake is going to hit,
            //if the snake hits wall(1) or a body part of the snake(3), then break the game loop
            CellType nextGridValue { grid.GetCell(Vector2Int{newSnakePosition.x, newSnakePosition.y}) };
            if (nextGridValue == CellType::Wall || (std::find(snakeBody.begin(), snakeBody.end(), newSnakePosition) != snakeBody.end()))
            {
                running = false;
                break;
            }
            //else if snake hits food(2), make the snake longer
            if (nextGridValue == CellType::Food)
                SnakeGrowing = true;
                    
        }
        // updates snake body part position
        grid.SetCell(newSnakePosition, CellType::Player);
        snakeBody[i] = newSnakePosition;
        // sets the current body position so the next position can read where it should go
        lastBodyPosition = oldSnakeBodyPartPosition;
    }
    
    grid.SetCell(lastBodyPosition, CellType::Empty);
    
   // if (SnakeGrowing)
   // {
   //     // creates a new snake body part and 
   //     // spawn in a new food at random position
   //     snakeBody.push_back(lastBodyPosition);
   //     SpawnFood(grid);
   //     ++gameScore;
   // }
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
        SaveFile::SaveHighscoreData(SaveFile::SaveData{name, gameScore});
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
    
    // snake movement
    Vector2Int directionValue { 1,0 };
    // creates the snake body and initializes it with it's head at the grid middle
    //std::vector<Vector2Int> snakeBody {Vector2Int{width/2,height/2}};
    
    Grid grid = Grid(width,height);
    Snake snake(Vector2Int(width/2,height/2), grid);
    
    Food foodPosition;
    
    // food position
    SpawnFood(grid, foodPosition);
    
    grid.Render();
    
    // set timer for how often the snake updates
    auto lastUpdate = std::chrono::steady_clock::now();
    std::chrono::milliseconds interval {200};
    Vector2Int temporaryDirectionValue = directionValue;
    
    // Snake game loop
    while (running)
    {
        auto now = std::chrono::steady_clock::now();
        // Gets the player input and stores the value inside temporaryDirectionValue
        //GetPlayerInput(running, directionValue, temporaryDirectionValue);
        
        GetPlayerInput(running, snake);
        
        if (now - lastUpdate >= interval)
        {
            grid.Render();
            
            bool grow {false};
            if (snake.Occupies(foodPosition.position))
                grow = true;
            
            snake.Move(grow);
            if (!snake.IsAlive())
                break;
            
            // UpdateSnakePosition(running, snakeBody, grid, temporaryDirectionValue, gameScore);
            lastUpdate = now;
        }
    }
    
    DrawGameOverScreen(gameScore);
}

void DrawLeaderboard()
{
    CLEAR_SCREEN
    std::vector<SaveFile::SaveData> saveFile {SaveFile::GetHighscoreFile()};
    
    std::cout << "===== High score =====\n\n";
    for (int i = 0; i < static_cast<int>(saveFile.size()); ++i)
    {
        std::cout <<
            i+1 << ". " << saveFile[i].name << ": " << saveFile[i].score << "\n"; 
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
    std::cout << "Press a number to choose what to do\n";
    std::cout << "1) Play Snake \n" 
        << "2) Controls/Help\n" 
        << "3) Leaderboard\n"
        << "4) Quit\n";
   
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
