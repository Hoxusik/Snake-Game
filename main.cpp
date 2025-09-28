#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "Defines.h"
#include "Tools.h"
#include "Grid.h"
#include "Snake.h"
#include "Dots.h"


int game(SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Surface* screen, SDL_Surface* charset)
{
    srand(time(NULL));
    int t1, t2, quit, frames, points;
    double delta, worldTime, fpsTimer, fps, distance, etiSpeed, speedUpTime;
    int snake_length = 2;
    SDL_Event event;

    char text[128];
    int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
    int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
    int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
    int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

    t1 = SDL_GetTicks();

    frames = 0;
    fpsTimer = 0;
    fps = 0;
    quit = 0;
    worldTime = 0;
    speedUpTime = 0;
    distance = 0;;
    points = 0;
    Grid grid(screen, MAP_WIDTH, MAP_HEIGHT);
    BlueDot* blueDot = new BlueDot(screen, &grid, rand() % MAP_WIDTH, rand() % MAP_HEIGHT);
    RedDot* redDot = new RedDot(screen, &grid, rand() % MAP_WIDTH, rand() % MAP_HEIGHT);
    Snake snake(screen, 5, 8, &grid, blueDot, redDot, &points);
    double snakeTime = 0;
    int retCode = 0;

    while (!quit) {
        if (!snake.isAlive()) {
            // Wyïż½wietl ekran koïż½ca gry
            SDL_FillRect(screen, NULL, czarny);
            sprintf(text, "Koniec gry! Zdobyles %d punktow N - Nowa gra, ESC - Wyjscie", points);
            DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, screen->h / 2, text, charset);

            SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
            SDL_RenderCopy(renderer, scrtex, NULL, NULL);
            SDL_RenderPresent(renderer);


            while (1) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_KEYDOWN) {
                        if (event.key.keysym.sym == SDLK_n) return 1;
                        if (event.key.keysym.sym == SDLK_ESCAPE) return 0;
                    }
                    else if (event.type == SDL_QUIT) {
                        return 0;
                    }
                }
            }
        }

        t2 = SDL_GetTicks();


        // w tym momencie t2-t1 to czas w milisekundach,
        // jaki uplynaïż½ od ostatniego narysowania ekranu
        // delta to ten sam czas w sekundach
        // here t2-t1 is the time in milliseconds since
        // the last screen was drawn
        // delta is the same time in seconds
        delta = (t2 - t1) * 0.001;
        t1 = t2;

        worldTime += delta;
        snakeTime += delta;
        speedUpTime += delta;
        redDot->UpdateTime(delta);
        SDL_FillRect(screen, NULL, czarny);


        fpsTimer += delta;
        if (fpsTimer > 0.5) {
            fps = frames * 2;
            frames = 0;
            fpsTimer -= 0.5;
        };

        // tekst informacyjny / info text
        //DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 36, czerwony, niebieski);
        //            "template for the second project, elapsed time = %.1lf s  %.0lf frames / s"
        sprintf(text, "czas trwania = %.1lf s | %d punktow | %.0lf fps | 1-4 A B C D", worldTime, points, fps);
        DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
        //            "Esc - exit, \030 - faster, \031 - slower"
        sprintf(text, "ESC - wyjscie");
        DrawString(screen, screen->w - screen->w / 8 - strlen(text) * 8 / 2, screen->h / 2, text, charset);
        sprintf(text, "N - nowa gra");
        DrawString(screen, screen->w - screen->w / 8 - strlen(text) * 8 / 2, screen->h / 2 + 10, text, charset);

        if (speedUpTime >= SPEED_UP_TIME)
        {
            speedUpTime = 0;
            double currentSpeed = snake.getSpeed();
            currentSpeed *= SPEED_UP_SCALE;
            snake.setSpeed(currentSpeed);
        }
        if (snakeTime > snake.getSpeed())
        {
            snake.Move();
            snakeTime = 0;
            if (rand() % HUNDRED_PERCENT < CHANCE_FOR_RED_DOT)
            {
                redDot->Respawn();
            }
        }

        grid.DrawBoard();

        snake.Draw();
        blueDot->Draw();
        redDot->Draw();

        SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
        //              SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, scrtex, NULL, NULL);
        SDL_RenderPresent(renderer);

        // obsïż½uga zdarzeïż½ (o ile jakieïż½ zaszïż½y) / handling of events (if there were any)
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
                else if (event.key.keysym.sym == SDLK_UP) {
                    if (snake.GetLastMove() != DOWN) snake.SetNextMove(UP);
                }
                else if (event.key.keysym.sym == SDLK_DOWN) {
                    if (snake.GetLastMove() != UP) snake.SetNextMove(DOWN);
                }
                else if (event.key.keysym.sym == SDLK_LEFT) {
                    if (snake.GetLastMove() != RIGHT) snake.SetNextMove(LEFT);
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    if (snake.GetLastMove() != LEFT) snake.SetNextMove(RIGHT);
                }
                else if (event.key.keysym.sym == SDLK_n)
                {
                    retCode = 1;
                    quit = 1;
                }
                else if (event.key.keysym.sym == SDLK_t)
                {
                    snake.addBody();
                }
                break;
            case SDL_QUIT:
                quit = 1;
                break;
            };

        };
        frames++;
    };
    return retCode;
}

// main
#ifdef __cplusplus
extern "C"
#endif

int main(int argc, char** argv) {
    int rc;

    SDL_Surface* screen, * charset;
    SDL_Texture* scrtex;
    SDL_Window* window;
    SDL_Renderer* renderer;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    // tryb peïż½noekranowy / fullscreen mode
    rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
    //      rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
    // //                                 &window, &renderer);
    if (rc != 0) {
        SDL_Quit();
        printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
        return 1;
    };

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_SetWindowTitle(window, "Snake");


    screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);


    // wyïż½ïż½czenie widocznoïż½ci kursora myszy
    SDL_ShowCursor(SDL_DISABLE);

    // wczytanie obrazka cs8x8.bmp
    charset = SDL_LoadBMP("./cs8x8.bmp");
    if (charset == NULL) {
        printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
        SDL_FreeSurface(screen);
        SDL_DestroyTexture(scrtex);
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    };
    SDL_SetColorKey(charset, true, 0x000000);

    int play = 1;
    while (play)
    {
        play = game(scrtex, renderer, screen, charset);
    }

    // zwolnienie powierzchni / freeing all surfaces
    SDL_FreeSurface(charset);
    SDL_FreeSurface(screen);
    SDL_DestroyTexture(scrtex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
};