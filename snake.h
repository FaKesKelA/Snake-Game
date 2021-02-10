#ifndef snake_h
#define snake_h

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "GridSys.h"
#include "ui.h"

typedef struct Movement
{
    bool up, down, left, right;
}Movement;

typedef struct SnakeSegments
{
    int angle;
    int x, y;
    struct SnakeSegments* previous;
    struct SnakeSegments* next;

}SnakeSegments;

typedef struct Snake
{
    int x, y;
    double Delay;
    double DelayEnd;
    int Length;
    struct SnakeSegments* body;
    int angle;
    Movement movement;
    
}Snake;

typedef struct Apple
{
   int x, y;
}Apple;

void SnakeMovement(Movement* mov, Snake* s, int rows, int columns, Tile** Grid, state* currentState);
void DrawSnakeSegment(SDL_Renderer* renderer, SDL_Texture* tex, Snake* segment, Tile** Grid);
void NewSegment(Snake* s);
void NewCords(Apple* a, Tile** Grid, int rows, int columns);
void handleKeyboard(int key, Movement* mov, bool* quitGame);
void freeSnake(Snake* s);

#endif /* snake_h */
