#pragma once
#include "Defines.h"
#include "Tools.h"

struct Cell {
    int x, y;
    int type; // -1 - sciana, 0 - nic, 1 - snake, 2 - niebieska, 3 - czerwona
};

class Grid
{

public:
    Grid(SDL_Surface* screen, int width, int height);
    ~Grid();
    void DrawBoard();
    Cell GetCell(int x, int y);
    void setCell(int x, int y, int type);

private:
    int width, height;
    SDL_Surface* screen;
    int baseX, baseY; // poczatkowe kordy grida
    Cell** grid = nullptr;

};