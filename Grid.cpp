#include "Grid.h"
Grid::Grid(SDL_Surface* screen, int width, int height)
{
    this->screen = screen;
    this->width = width;
    this->height = height;
    int w, h;
    w = GRID_SIZE * width;
    h = GRID_SIZE * height;
    baseX = SCREEN_WIDTH / 2 - w / 2;
    baseY = SCREEN_HEIGHT / 2 - h / 2;

    grid = new Cell * [width];
    for (int i = 0; i < width; i++)
    {
        grid[i] = new Cell[height];
    }
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            grid[i][j].x = baseX + i * GRID_SIZE;
            grid[i][j].y = baseY + j * GRID_SIZE;
            grid[i][j].type = EMPTY;
        }
    }
}

Grid::~Grid()
{
    for (int i = 0; i < width; i++)
    {
        if (grid[i] != nullptr)
        {
            delete grid[i];
        }
    }
    if (grid != nullptr)
    {
        delete grid;
    }
}

Cell Grid::GetCell(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        Cell temp;
        temp.x = WALL;
        temp.y = WALL;
        temp.type = WALL;
        return temp;
    }
    return grid[x][y];
}

void Grid::setCell(int x, int y, int type)
{
    grid[x][y].type = type;
}

void Grid::DrawBoard()
{
    DrawRectangle(screen, baseX, baseY, GRID_SIZE * width, GRID_SIZE * height, 0xff0000, 0x000000);

}