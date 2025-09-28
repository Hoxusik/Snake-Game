#include "Snake.h"

Snake_Body::Snake_Body(SDL_Surface* screen, int x, int y, Grid* grid)
{
    this->screen = screen;
    this->x = x;
    this->y = y;
    this->grid = grid;
}

Snake_Body::~Snake_Body()
{

}

int Snake_Body::getLastX()
{
    return lastX;
}

int Snake_Body::getLastY()
{
    return lastY;
}

int Snake_Body::getX()
{
    return x;
}

int Snake_Body::getY()
{
    return y;
}

void Snake_Body::Draw()
{
    DrawRectangle(screen, grid->GetCell(x, y).x, grid->GetCell(x, y).y, GRID_SIZE, GRID_SIZE, color, color);
}

void Snake_Body::Update(int x, int y)
{
    this->lastX = this->x;
    this->lastY = this->y;
    this->x = x;
    this->y = y;

}

Snake::Snake(SDL_Surface* screen, int x, int y, Grid* grid, BlueDot* blueDot, RedDot* redDot, int* points)
{
    this->screen = screen;
    this->x = x;
    this->y = y;
    this->grid = grid;
    this->body = new Snake_Body * [snakeLength];
    this->blueDot = blueDot;
    this->redDot = redDot;
    this->points = points;
    addBody();
}

Snake::~Snake()
{
    for (int i = 0; i < snakeLength; i++)
    {
        if (body[i] != nullptr)
        {
            delete body[i];
        }
    }
    if (body != nullptr)
        delete body;
}

void Snake::Draw()
{
    DrawRectangle(screen, grid->GetCell(x, y).x, grid->GetCell(x, y).y, GRID_SIZE, GRID_SIZE, color, color);
    for (int i = 0; i < snakeLength; i++)
    {
        body[i]->Draw();
    }
}

void Snake::SetNextMove(Direction direction)
{
    this->nextMove = direction;
}

Direction Snake::GetNextMove()
{
    return nextMove;
}

void Snake::SetLastMove(Direction direction)
{
    this->lastMove = direction;
}

Direction Snake::GetLastMove()
{
    return lastMove;
}


void Snake::Move()
{
    for (int i = snakeLength - 1; i > 0; i--)
    {
        body[i]->Update(body[i - 1]->getX(), body[i - 1]->getY()); //przekazywanie kordow z blizszego ciala do dalszego (od glowy)
    }
    if (body[snakeLength - 1]->getLastX() != -1)
    {
        grid->setCell(body[snakeLength - 1]->getLastX(), body[snakeLength - 1]->getLastY(), 0);
    }
    body[0]->Update(x, y);



    switch (nextMove)
    {
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    default:
        break;
    }

    Cell cell = grid->GetCell(x, y);
    if (cell.type == WALL)
    {
        switch (nextMove) //cofanie zmiany kordow, zeby sprawdzic gdzie ma sie ruszyc
        {
        case UP:
            y++;
            break;
        case DOWN:
            y--;
            break;
        case LEFT:
            x++;
            break;
        case RIGHT:
            x--;
            break;
        default:
            break;
        }

        if (nextMove == UP && lastMove != DOWN)
        {
            cell = grid->GetCell(x + 1, y);
            if (cell.type != WALL && cell.type != SNAKE)
            {
                x++;
                nextMove = RIGHT;
            }
            else
            {
                cell = grid->GetCell(x - 1, y); //pobranie jednej komorki, zeby sprawdzic jego typ
                if (cell.type != WALL && cell.type != SNAKE)//sprawdzanie czy nastepne pole jest wolne
                {
                    x--;
                    nextMove = LEFT; //jesli nie zmienimy kierunku to nadal bedzie sie poruszal w tym samym
                }
                else
                {
                    alive = false; //jesli brak mozliwego ruchu to ginie
                }
            }
        }
        else if (nextMove == DOWN && lastMove != UP)
        {
            cell = grid->GetCell(x - 1, y);
            if (cell.type != WALL && cell.type != SNAKE)
            {
                x--;
                nextMove = LEFT;
            }
            else
            {
                cell = grid->GetCell(x + 1, y);
                if (cell.type != WALL && cell.type != SNAKE)
                {
                    x++;
                    nextMove = RIGHT;
                }
                else
                {
                    alive = false;
                }
            }
        }
        else if (nextMove == LEFT && lastMove != RIGHT)
        {
            cell = grid->GetCell(x, y - 1);
            if (cell.type != WALL && cell.type != SNAKE)
            {
                y--;
                nextMove = UP;
            }
            else
            {
                cell = grid->GetCell(x, y + 1);
                if (cell.type != WALL && cell.type != SNAKE)
                {
                    y++;
                    nextMove = DOWN;
                }
                else
                {
                    alive = false;
                }
            }

        }
        else if (nextMove == RIGHT && lastMove != LEFT)
        {
            cell = grid->GetCell(x, y + 1);
            if (cell.type != WALL && cell.type != SNAKE)
            {
                y++;
                nextMove = DOWN;
            }
            else
            {
                cell = grid->GetCell(x, y - 1);
                if (cell.type != WALL && cell.type != SNAKE)
                {
                    y--;
                    nextMove = UP;
                }
                else
                {
                    alive = false; //jesli brak mozliwego ruchu to ginie
                }
            }
        }
    }
    else if (cell.type == SNAKE) //jesli w komorce jest waz
    {
        alive = false;
    }
    else if (cell.type == BLUE_DOT) //jesli w komorce jest niebieska kropka
    {
        addBody();
        blueDot->Kill(); //znikanie kropki
        *points += POINTS_PER_BLUE_DOT;
    }
    else if (cell.type == RED_DOT) //jesli w komorce jest czerwona kropka
    {
        redDot->Kill(); //znikanie kropki z mapy
        if (rand() % HUNDRED_PERCENT < FIFTY_PERCENT) //50% szans na  zmniejszenie predkosci lub na skrocenie weza
        {
            this->speed *= SLOW_DOWN_SCALE;
        }
        else
        {
            for (int i = 0; i < SHORTENS_UNITS_OF_BODY; i++)
            {
                RemoveBody();
            }
        }
        *points += POINTS_PER_RED_DOT;
    }
    grid->setCell(x, y, 1);
    lastMove = nextMove;
}

double Snake::getSpeed()
{
    return speed;
}

void Snake::setSpeed(double speed)
{
    this->speed = speed;
}

void Snake::addBody()
{
    Snake_Body** temp = new Snake_Body * [snakeLength + 1];
    for (int i = 0; i < snakeLength; i++)
    {
        temp[i] = body[i];
    }
    if (snakeLength > 0)
    {
        temp[snakeLength] = new Snake_Body(screen, temp[snakeLength - 1]->getLastX(), temp[snakeLength - 1]->getLastY(), grid);
        grid->setCell(temp[snakeLength]->getX(), temp[snakeLength]->getY(), 1);
    }
    else
    {
        temp[snakeLength] = new Snake_Body(screen, x, y + 1, grid); //pierwszy ogon, ktory sie pojawi bedzie pod glowa
        grid->setCell(temp[snakeLength]->getX(), temp[snakeLength]->getY(), 1);
    }
    body = temp;
    snakeLength++;
}

bool Snake::isAlive()
{
    return alive;
}

void Snake::RemoveBody()
{
    if (snakeLength <= 1)
        return;
    Snake_Body** temp = new Snake_Body * [snakeLength - 1];
    for (int i = 0; i < snakeLength - 1; i++)
    {
        temp[i] = body[i];

    }
    if (snakeLength > 0)
    {
        grid->setCell(body[snakeLength - 1]->getX(), body[snakeLength - 1]->getY(), 0);
        delete body[snakeLength - 1];
    }
    body = temp;
    snakeLength--;
}