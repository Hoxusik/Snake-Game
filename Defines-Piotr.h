#pragma once
#define SCREEN_WIDTH    640 //szerokosc ekranu
#define SCREEN_HEIGHT   480 //wysokosc ekranu
#define GRID_SIZE 25 //rozmiar jednej komorki 
#define MAP_WIDTH 10 //6-14
#define MAP_HEIGHT 14 //10-16
#define SPEED_UP_TIME 10 //czas w sekundach, po ktorym bedzie przyspiesznie
#define SPEED_UP_SCALE 0.2 //20% szybciej
#define SLOW_DOWN_SCALE 1.15 //15% wolniej
#define RED_DOT_TIME 5 //czas wyswietlania czerwonej kropki
#define GRAY 0x808080 //kolor
#define SHORTENS_UNITS_OF_BODY 2 //ile fragmentow ciala zostanie uciete po zjedzeniu czerwonej kropki?
#define POINTS_PER_BLUE_DOT 50 
#define POINTS_PER_RED_DOT 25
#define CHANCE_FOR_RED_DOT 5 //how much % for red dot
#define HUNDRED_PERCENT 100 // 100%
#define FIFTY_PERCENT 50 //50%
#define WALL -1 //typ sciany
#define SNAKE 1 //typ snake'a
#define EMPTY 0 //typ pustego pola
#define BLUE_DOT 2 //typ niebieskiej kropki
#define RED_DOT 3 //typ czerwonej kropki
extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}