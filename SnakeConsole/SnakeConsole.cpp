#include <iostream>
#include <random>
#include <vector>



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

bool IsInsideGrid(int x, int y, std::vector<std::vector<int>>& gridArea)
{
    return x > 0 && x < gridArea.size()
        && y > 0 && y < gridArea[0].size();
}

std::vector<std::vector<int>> GenerateGrid(int width, int height)
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

void SpawnFood(std::vector<std::vector<int>>& grid_area)
{
    // Gets a random pos in the grid_area
    int randX { GetRandomNumber(grid_area.size()) - 1  };
    int randY { GetRandomNumber(grid_area[0].size()) - 1 };
    
    // checks if the random pos is inside the grid_area
    // if not rerun this function until it has found a valid pos
    // else place the food at coordinate
    if (!IsInsideGrid(randX, randY, grid_area))
    {
        SpawnFood(grid_area);
        return;
    }
    grid_area[randX][randY] = 2;
}

void DisplayPlayArea(std::vector<std::vector<int>>& gridArea)
{
    for (int y = 0; y < gridArea[0].size(); y++)
    {
        for (int x = 0; x < gridArea.size(); x++)
        {
            // is a wall
            if (gridArea[x][y] == 1)
                std::cout << '*';
            // is food
            else if (gridArea[x][y] == 2)
                std::cout << '0';
            // white space
            else
                std::cout << ' ';
        }
        std::cout << '\n';
    }
}




int main(int argc, char* argv[])
{
    int width { 20 };
    int height { 10 };
    
    std::vector<std::vector<int>> gridArea {GenerateGrid(width,height) };
    SpawnFood(gridArea);
    DisplayPlayArea(gridArea);
    
    std::cin >> std::ws;
    return 0;
}
