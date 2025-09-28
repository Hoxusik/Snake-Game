#include "Dots.h"


BlueDot::BlueDot(SDL_Surface* screen, Grid* grid, int x, int y)
{
    image = SDL_LoadBMP("./blue_dot25.bmp");
    this->screen = screen;
    this->grid = grid;
    this->x = x;
    this->y = y;
    grid->setCell(x, y, 2);
}

BlueDot::~BlueDot()
{
    SDL_FreeSurface(image);
}

void BlueDot::Draw()
{
    DrawSurfaceNoCenter(screen, image, grid->GetCell(x, y).x, grid->GetCell(x, y).y);
}

void BlueDot::Kill()
{
    x = rand() % MAP_WIDTH;
    y = rand() % MAP_HEIGHT;
    if (grid->GetCell(x, y).type == 0)
    {
        grid->setCell(x, y, 2);
    }
    else
    {
        this->Kill();
    }
}


RedDot::RedDot(SDL_Surface* screen, Grid* grid, int x, int y)
{
    image = SDL_LoadBMP("./red_dot25.bmp");
    this->screen = screen;
    this->grid = grid;
    this->x = x;
    this->y = y;
    this->alive = false;
}

RedDot::~RedDot()
{
    SDL_FreeSurface(image);
}

void RedDot::Draw()
{
    if (alive)
    {
        DrawSurfaceNoCenter(screen, image, grid->GetCell(x, y).x, grid->GetCell(x, y).y);
        DrawTimer();
    }
}

void RedDot::Kill()
{
    if (alive)
    {
        alive = false;
    }
}

void RedDot::Respawn()
{
    if (!alive)
    {
        x = rand() % MAP_WIDTH;
        y = rand() % MAP_HEIGHT;
        if (grid->GetCell(x, y).type == 0)
        {
            grid->setCell(x, y, 3);
            alive = true;
            time = RED_DOT_TIME;
        }
        else //ponowny respawn jesli zajeta komorka
        {
            this->Respawn();
        }

    }
}

void RedDot::UpdateTime(double time)
{
    if (alive)
    {
        this->time -= time;
        if (this->time <= 0)
        {
            alive = false;
            grid->setCell(x, y, 0);
            this->time = 0;
        }
    }
}

void RedDot::DrawTimer()
{
    if (alive)
    {
        int y = SCREEN_HEIGHT / 2 - GRID_SIZE * MAP_HEIGHT / 2 - 15;
        int x = SCREEN_WIDTH / 2 - GRID_SIZE * MAP_WIDTH / 2 + GRID_SIZE;
        int w = GRID_SIZE * MAP_WIDTH - GRID_SIZE * 2;
        int h = 10;
        double percent = time / RED_DOT_TIME;
        int w2 = (w - 2) * percent; //odejmujemy pixel od lewej i prawej
        DrawRectangle(screen, x, y, w, h, GRAY, 0x000000);
        DrawRectangle(screen, x + 1, y + 1, w2, h - 2, 0xFF0000, 0xFF0000); //+1, bo obramowka szara, ktora ma 1 pixel i nie chcemy na nia wjechac, h-2, bo odejmujemy 1 pixel od gory i od dolu
    }
}