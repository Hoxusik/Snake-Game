#pragma once
#include "Defines.h"
#include "Tools.h"
#include "Grid.h"
#include <stdlib.h>

class BlueDot
{
public:
    BlueDot(SDL_Surface* screen, Grid* grid, int x, int y);
    ~BlueDot();
    void Draw();
    void Kill();

private:
    SDL_Surface* image;
    SDL_Surface* screen;
    Grid* grid;
    int x, y;
};

class RedDot
{
public:
    RedDot(SDL_Surface* screen, Grid* grid, int x, int y);
    ~RedDot();
    void Draw();
    void Kill();
    void Respawn();
    void UpdateTime(double time);


private:
    SDL_Surface* image;
    SDL_Surface* screen;
    Grid* grid;
    int x, y;
    bool alive = true;
    double time = 0;
    void DrawTimer();
};