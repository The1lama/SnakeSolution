#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <conio.h>
#include <fstream>
#include <string>
#include <Windows.h>
#include <limits>


#pragma region Defines

#define CLEAR_SCREEN std::cout << "\x1b[2J\x1b[H";
#define ToInt(x)  static_cast<int>(x)
#define twoDArray std::vector<std::vector<CellType>>

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

struct GameSettings
{
    // Game board size
    int width{20};
    int height{10};
    // Draw characters 
    char wall{'#'};
    char food{'*'};
    char player{'@'};
    char whiteSpace{' '};
};

enum class GameState
{
    MainMenu,
    PlaySnake,
    Help,
    Highscore,
    Quit,
};

enum class CellType
{
    Empty,
    Wall,
    Food,
    Player,
};

#pragma region Grid

void RenderGrid(const twoDArray& grid_area, const std::vector<Vector2Int>& snakeBody, const Vector2Int& foodPosition)
{
    Helper::ClearSnakeScreen();
    for (int y = 0; y < ToInt(grid_area[0].size()); y++)
    {
        for (int x = 0; x < ToInt(grid_area.size()); x++)
        {
            if (std::find(snakeBody.begin(), snakeBody.end(), Vector2Int{x,y}) != snakeBody.end())
            {
                std::cout << '@';
                continue;
            }
            if (Vector2Int{x,y} == foodPosition)
            {
                std::cout << '*';
                continue;
            }
            
            if (grid_area[x][y] == CellType::Empty)
                std::cout << ' ';
            else if (grid_area[x][y] == CellType::Wall)
                std::cout << '#';
        }
        std::cout << '\n';
    }
}

bool IsInsideGrid(const int x,const int y, const twoDArray& gridArea)
{
    return x > 0 && x < ToInt(gridArea.size()-1)
        && y > 0 && y < ToInt(gridArea[0].size()-1);
}

twoDArray GenerateGrid(int width, int height)
{
    // creates a 2d dynamic array 
    std::vector<std::vector<CellType>> gridVectors;
    // sets the size of the 2d dynamic array
    gridVectors.resize(width, std::vector<CellType>(height));
    
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (x == 0 || x == width -1 || y == 0 || y == height -1)    // sets the cell type to wall 
                gridVectors[x][y] = CellType::Wall;
            else 
                gridVectors[x][y] = CellType::Empty;
        }
    }
    
    return gridVectors;
}

#pragma endregion Grid

// spawns food at a random grid position
Vector2Int SpawnFood(const twoDArray& grid_area, const std::vector<Vector2Int>& snakeBody)
{
    // Gets a random pos in the grid_area
    int randX {Helper::GetRandomNumber(ToInt(grid_area.size())) - 1  };
    int randY {Helper::GetRandomNumber(ToInt(grid_area[0].size())) - 1 };
    
    // checks if the random pos is inside the grid_area
    // if not rerun this function until it has found a valid pos
    // else place the food at coordinate
    if (!IsInsideGrid(randX, randY, grid_area))
    {
        return SpawnFood(grid_area, snakeBody);
    }
    // if the food spawns inside a wall or player it finds a new random place to spawn
    CellType vectorValue { grid_area[randX][randY] };
    if (vectorValue == CellType::Wall || ((std::find(snakeBody.begin(), snakeBody.end(), Vector2Int{randX,randY}) != snakeBody.end())))
    {
        return SpawnFood(grid_area, snakeBody);
    }
    return Vector2Int{randX, randY};
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

// Updates the snake position on grid 
void UpdateSnakePosition(bool& running, std::vector<Vector2Int>& snakeBody, 
    const std::vector<std::vector<CellType>>& gridArea, Vector2Int& temporaryDirectionValue, Vector2Int& foodPosition, int& gameScore)
{
    // for initializing the snake start body position, Start from snake head
    Vector2Int lastBodyPosition = snakeBody[0];
    bool SnakeGrowing { false };
    // for moving the whole snake
    for (int i = 0; i < ToInt(snakeBody.size()); ++i)
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
            CellType nextGridValue { gridArea[newSnakePosition.x][newSnakePosition.y] };
            if (nextGridValue == CellType::Wall || (std::find(snakeBody.begin(), snakeBody.end(), newSnakePosition) != snakeBody.end()))
            {
                running = false;
                break;
            }
            //else if snake hits food(2), make the snake longer
            if (newSnakePosition == foodPosition)
                SnakeGrowing = true;
                    
        }
        // updates snake body part position
        snakeBody[i] = newSnakePosition;
        // sets the current body position so the next position can read where it should go
        lastBodyPosition = oldSnakeBodyPartPosition;
    }
    if (SnakeGrowing)
    {
        // creates a new snake body part and 
        // spawn in a new food at random position
        snakeBody.push_back(lastBodyPosition);
        foodPosition = SpawnFood(gridArea, snakeBody);
        ++gameScore;
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
    std::vector<Vector2Int> snakeBody {Vector2Int{width/2,height/2}};
    
    twoDArray gridArea {GenerateGrid(width,height) };
    
    // food position
    Vector2Int foodPosition = SpawnFood(gridArea, snakeBody);
    
    // SpawnFood(gridArea, snakeBody);
    RenderGrid(gridArea, snakeBody, foodPosition);
    
    // set timer for how often the snake updates
    auto lastUpdate = std::chrono::steady_clock::now();
    std::chrono::milliseconds interval {200};
    Vector2Int temporaryDirectionValue = directionValue;
    
    // Snake game loop
    while (running)
    {
        auto now = std::chrono::steady_clock::now();
        // Gets the player input and stores the value inside temporaryDirectionValue
        GetPlayerInput(running, directionValue, temporaryDirectionValue);
        
        if (now - lastUpdate >= interval)
        {
            UpdateSnakePosition(running, snakeBody, gridArea, temporaryDirectionValue, foodPosition, gameScore);
            lastUpdate = now;
            RenderGrid(gridArea, snakeBody, foodPosition);
            if (directionValue != temporaryDirectionValue)  // Sets the temporaryDirectionValue to the direction value, if it has changed
                directionValue = temporaryDirectionValue;   
        }
    }
    
    DrawGameOverScreen(gameScore);
}

void DrawLeaderboard()
{
    CLEAR_SCREEN
    std::vector<SaveFile::SaveData> saveFile {SaveFile::GetHighscoreFile()};
    
    std::cout << "===== High score =====\n\n";
    for (int i = 0; i < ToInt(saveFile.size()); ++i)
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

void DrawMainMenu(GameState& currentState)
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
            DrawMainMenu(currentState);
            int choise = ReadIntInRange(1,4);
            if (choise == 1) currentState = GameState::PlaySnake;
            if (choise == 2) currentState = GameState::Help;
            if (choise == 3) currentState = GameState::Highscore;
            if (choise == 4) currentState = GameState::Quit;
            break;
        case GameState::PlaySnake:
            PlaySnake();
            currentState = GameState::MainMenu;
            break;
        case GameState::Help:
            DrawHelpMenu();
            currentState = GameState::MainMenu;
            break;
        case GameState::Highscore:
            DrawLeaderboard();    
            currentState = GameState::MainMenu;
            break;
        case GameState::Quit:
            currentState = GameState::Quit;
            break;
        }
    }
}

int main(int argc, char* argv[])
{
    SpawnMainMenu();
    return 0;
}
