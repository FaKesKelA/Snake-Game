#ifndef ui_h
#define ui_h
#include <stdio.h>
#include "GridSys.h"
#include <stdbool.h>
#include <string.h>
#include "SDL2_ttf/SDL_ttf.h"

typedef struct textStruct{
    SDL_Rect box;
    char self[32];
    int scale;
    SDL_Color color;
}textStruct;

typedef struct button{
    SDL_Rect position;
    textStruct text;
    bool clicked;
    SDL_Color color[3];
}button;

typedef enum state{
    mainMenu, loading, inGame, quit, dead
}state;

void inMainMenu(state* currentState, int* mapType, Tile** Grid, int columns, int rows, SDL_Renderer* renderer, TTF_Font* font);
static void buttonAction(button* btn, SDL_Event* ev);
static button buttonCreator(SDL_Rect gridPosition, int width, int height, char* text, int scaleSize);
void textRenderer(SDL_Renderer* renderer, TTF_Font* font, textStruct* word);
void deadScreen(Tile** Grid, int columns, int rows, state* currentState, int score, SDL_Renderer* renderer, TTF_Font* font);
textStruct textCreator(SDL_Rect box, char self[16], int scale, SDL_Color color);


#endif /* ui_h */
