#pragma once
#include <stdlib.h>
#include "Defines.h"
#include "Tools.h"
#include "Grid.h"
#include "Dots.h"
enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};
class Snake_Body
{
public:
    Snake_Body(SDL_Surface* screen, int x, int y, Grid* grid);
    ~Snake_Body();
    int getLastX();
    int getLastY();
    int getX();
    int getY();
    void Draw();
    void Update(int x, int y);

private:
    int x, y;
    Uint32 color = 0x3CB371;
    SDL_Surface* screen;
    int lastX = -1, lastY = -1;
    Grid* grid;
};

class Snake
{
public:
    Snake(SDL_Surface* screen, int x, int y, Grid* grid, BlueDot* blueDot, RedDot* redDot, int* points);
    ~Snake();
    void Draw();
    void SetNextMove(Direction direction);
    Direction GetNextMove();
    void SetLastMove(Direction direction);
    Direction GetLastMove();
    void Move();
    double getSpeed();
    void setSpeed(double speed);
    void addBody();
    bool isAlive();

private:
    SDL_Surface* screen;
    int x, y;
    Grid* grid;
    Uint32 color = 0x008000;
    int snakeLength = 0;
    Snake_Body** body = nullptr;
    Direction nextMove = UP;
    Direction lastMove = UP;
    double speed = 0.2;
    //void addBody();
    bool alive = true;
    BlueDot* blueDot;
    RedDot* redDot;
    void RemoveBody();
    int* points;
};
