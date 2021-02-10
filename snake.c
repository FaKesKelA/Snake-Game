#include "snake.h"

// This function is for the movement of the snake
void SnakeMovement(Movement* mov, Snake* s, int rows, int columns, Tile** Grid, state* currentState)
{
    
    int x = 0, y = 0;
// the movement depends on previous directions as well so the snake cannot go into its self, for example if the snake went left it cannot go to the right after that
    if(mov->right)
    {
        if(!s->movement.left)
        {
            x += 1;
            s->movement.right = true;
            s->movement.up = false;
            s->movement.down = false;
            s->movement.left = false;
            s->angle = 0;

        }
        else
        {
            x -= 1;
        }
    }
    if(mov->left)
    {
        if(!s->movement.right)
        {
            x -= 1;
            s->movement.right = false;
            s->movement.up = false;
            s->movement.down = false;
            s->movement.left = true;
            s->angle = 180; // this is for the head to face other directions, so the texture does not face only the right side

        }
        else
        {
            x += 1;
        }
    }
    if(mov->up)
    {
        if(!s->movement.down)
        {
            y -= 1;
            s->movement.right = false;
            s->movement.up = true;
            s->movement.down = false;
            s->movement.left = false;
            s->angle = 270;

        }
        else
        {
            y += 1;
        }
    }
    
    if (mov->down)
    {
        if(!s->movement.up)
        {
            y += 1;
            s->movement.right = false;
            s->movement.up = false;
            s->movement.down = true;
            s->movement.left = false;
            s->angle = 90;

        }
        else
        {
            y -= 1;
        }
    }
// if the Grids tile has any of the parts of the snake and it goes into its self, or a obstacle, the program counts that as the end of the game
    if(Grid[s->y][s->x].hasSnake || Grid[s->y][s->x].collision)
    {
        *currentState = dead;
        s->Delay = 0;
        freeSnake(s);
        return;
    }
    SnakeSegments* probe = s->body;

    if(probe == NULL)
    {
        s->x += x;
        s->y += y;
        return;
    }
    if(probe->next == NULL)
    {
        Grid[probe->y][probe->x].hasSnake = 0;
        probe->x = s->x;
        probe->y = s->y;
        probe->angle = s->angle;
        Grid[probe->y][probe->x].hasSnake = 1;
    }
    while(probe->next != NULL)
    {
        probe = probe->next;
        Grid[probe->y][probe->x].hasSnake = 0;
    }
    while (probe->previous != NULL)
    {
        probe->x = probe->previous->x;
        probe->y = probe->previous->y;
        probe->angle = probe->previous->angle;
        Grid[probe->y][probe->x].hasSnake = 1;
        probe = probe->previous;
        
    }
    
    probe->x = s->x;
    probe->y = s->y;
    probe->angle = s->angle;
    
    s->x += x;
    s->y += y;
}
// This is the function that draws out the snake's body pieces
void DrawSnakeSegment(SDL_Renderer* renderer, SDL_Texture* tex, Snake* segment, Tile** Grid)
{
    SnakeSegments* probe = segment->body;
    while(probe != NULL)
    {
        if(probe->y != 0 && probe->x != 0)
            SDL_RenderCopyEx(renderer, tex, NULL, &Grid[probe->y][probe->x].location, probe->angle, NULL, 0);
        probe = probe->next;
    }
    
}
//This is the function that adds a new body part to the snake when it collides with an apple
void NewSegment(Snake* s)
{
    s->Length++;
    s->Delay *= 1.05;
    SnakeSegments* segment = (SnakeSegments*)malloc(sizeof(SnakeSegments));
    SnakeSegments* probe = s->body;
    segment->next = NULL;
    segment->previous = NULL;
    segment->x = 0;
    segment->y = 0;
    segment->angle = 0;
    if(probe == NULL)
    {
        s->body = segment;
        return;
    }
    segment->next = probe;
    probe->previous = segment;
    s->body = segment;
}
//This is the function that generates a new position to the apple when it gets eaten by the snake
void NewCords(Apple* a, Tile** Grid, int rows, int columns)
{
    Grid[a->y][a->x].hasApple = 0;
   
    do
    {
        a->x = rand() % rows;
        a->y = rand() % columns;
    }
    while(Grid[a->y][a->x].hasSnake || Grid[a->y][a->x].collision);
    
    Grid[a->y][a->x].hasApple = 1;
}
// This is the function that handles the movement with keyboard input, the user can use the arrows and also the WASD keys to move the snake
void handleKeyboard(int key, Movement* mov, bool* quitGame)
{
    switch (key)
    {
        case SDLK_LEFT:
        case SDLK_a:
            mov->left = true;
            mov->right = false;
            mov->up = false;
            mov->down = false;
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            mov->right = true;
            mov->left = false;
            mov->up = false;
            mov->down = false;
            break;
        case SDLK_UP:
        case SDLK_w:
            mov->up = true;
            mov->left = false;
            mov->right = false;
            mov->down = false;
            break;
        case SDLK_DOWN:
        case SDLK_s:
            mov->down = true;
            mov->left = false;
            mov->right = false;
            mov->up = false;
            break;
        case SDLK_ESCAPE:
            *quitGame = true;
            break;
    }
      
}

void freeSnake(Snake* s)
{
    SnakeSegments* probe = s->body;
    SnakeSegments* temp = NULL;
    while(probe != NULL)
    {
        temp = probe->next;
        free(probe);
        probe = temp;
    }
    s->body = NULL;
}


