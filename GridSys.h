#ifndef GridSys_h
#define GridSys_h
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>



typedef struct Tile
{
    SDL_Rect location;
    int hasApple;
    int collision;
    int hasSnake;
}Tile;

Tile** Area(SDL_Rect sqr, int rows, int columns, Tile** Grid);
void loadMap(Tile** Grid, int rows, int columns, int mapNumber);
void freeArea(Tile** Grid);

#endif /* GridSys_h */
