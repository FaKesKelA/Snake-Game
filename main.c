#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#include <time.h>
#include "GridSys.h"
#include "ui.h"
#include "snake.h"

typedef struct contextObject{
	
	SDL_Texture* texture;
	SDL_Texture* appletex;
	SDL_Texture* snakehead;
	SDL_Texture* grass;
	SDL_Texture* tail;
	SDL_Texture* wall;
	TTF_Font* font;
	
}contextObject;

// Starting SDL
bool sdl_init(int width, int height);
bool loadMedia(SDL_Renderer* renderer, contextObject* assets);
void quitSDL(contextObject* assets);

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int main(int argc, char* argv[])
{
    bool quitGame = false;
	int width = 720;
	int height = 720;
    int rows = 16;
    int columns = 16;
	contextObject assets;
	int mapType = 0;
	
	// FPS limiter - for limiting CPU usage
	const int FPS = 60;
	const int frameDelay = 1000/FPS;
	unsigned int frameTime = 0;
	Uint32 frameStart = 0;
	
	// Creating the object
	Apple a;
	Snake s;
	Movement mov;
	
	// Loads SDL, Assets
    if(!sdl_init(width, height))
        quitGame = true;
	
	if(!loadMedia(renderer, &assets))
		quitGame = true;
	
    Tile** Grid = NULL;
    SDL_Rect square ={ 0, 0, width, height};
    
    Grid = Area(square, rows, columns, Grid);
	if(Grid == NULL)
		quitGame = true;
	
	srand(time(0));

	state currentState = mainMenu;
	    
	// Game loop
    while(!quitGame){
		switch(currentState)
		{
			case mainMenu:
				inMainMenu(&currentState,&mapType, Grid, columns, rows, renderer, assets.font);
				break;
			case loading:
				loadMap(Grid, rows, columns, mapType);
				
				s.x = columns/2;
				s.y = rows / 3;
				s.Delay = 0.03;
				s.DelayEnd = 0;
				s.body = NULL;
				s.Length = 0;
				Movement tempMov = {0, 0, 0, 0};
				s.movement = tempMov;
				
				a.x = 0;
				a.y = 0;
				NewCords(&a, Grid, rows, columns);
				mov.down = mov.up = mov.right = mov.left = false;
				currentState = inGame;
				break;
			case inGame:
				while(currentState == inGame)
				{
					frameStart = SDL_GetTicks();
					SDL_Event event;
					while(SDL_PollEvent(&event))
					{
						switch (event.type)
						{
							case SDL_KEYDOWN:
								handleKeyboard(event.key.keysym.sym, &mov, &quitGame);
								break;
							case SDL_QUIT:
								currentState = quit;
								break;
							default:
								break;
						}
					}
					if(currentState != inGame)
						break;
					// Slows snake movement
					s.DelayEnd += s.Delay;
					if(s.DelayEnd >= 1)
					{
						s.DelayEnd = 0;
						SnakeMovement(&mov, &s, rows, columns, Grid, &currentState);
					}
					// Snake eats apple (collision)
					if(Grid[s.y][s.x].hasApple)
					{
						NewCords(&a, Grid, rows, columns);
						NewSegment(&s);
					}
					char scoreText[10] = "";
					sprintf(scoreText, "Score: %02d", s.Length);
					SDL_Color white = {255, 255, 255, 255};
					textStruct score = textCreator(Grid[0][columns/2-1].location, scoreText, 3, white);
					
					// Rendering texture to the screen
					SDL_RenderClear(renderer);
					for(int y = 0; y < rows; y++)
					{
						for(int x = 0; x < columns; x++)
						{
							if(Grid[y][x].collision)
								SDL_RenderCopy(renderer, assets.wall, NULL, &Grid[y][x].location);
							else
								SDL_RenderCopy(renderer, assets.grass, NULL, &Grid[y][x].location);
						}
					}
					SDL_RenderCopy(renderer, assets.appletex, NULL, &Grid[a.y][a.x].location);
					SDL_RenderCopyEx(renderer, assets.snakehead, NULL, &Grid[s.y][s.x].location, s.angle, NULL, 0);
					DrawSnakeSegment(renderer, assets.tail, &s, Grid);
					textRenderer(renderer, assets.font, &score);
					SDL_RenderPresent(renderer);
					// End of rendering
					// Capping frames/second
					frameTime = SDL_GetTicks() - frameStart;
					if(frameDelay > frameTime)
						SDL_Delay(frameDelay - frameTime);
				}
				break;
			case dead:
				deadScreen(Grid, columns, rows, &currentState, s.Length, renderer, assets.font);
				break;
			case quit:
				quitGame = true;
				break;
		}
    }
	// Cleanup
    freeArea(Grid);
	quitSDL(&assets);
	return 0;
}


bool sdl_init(int width, int height)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("SDL not starting: %s", SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == NULL)
    {
        SDL_Log("The window could not be started: %s", SDL_GetError());
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL)
    {
        SDL_Log("the viewer could not be started: %s", SDL_GetError());
        return false;
    }
    SDL_RenderClear(renderer);
	
	if(TTF_Init() < 0)
	{
		SDL_Log("SDL TTF not starting: %s", SDL_GetError());
		return false;
	}
	
    return true;
}

bool loadMedia(SDL_Renderer* renderer, contextObject* assets)
{
	assets->texture = IMG_LoadTexture(renderer, "Assets/test.png");
	assets->appletex = IMG_LoadTexture(renderer, "Assets/apple.png");
	assets->snakehead = IMG_LoadTexture(renderer, "Assets/snake.png");
	assets->grass = IMG_LoadTexture(renderer, "Assets/grass.png");
	assets->tail = IMG_LoadTexture(renderer, "Assets/snake_body.png");
	assets->wall = IMG_LoadTexture(renderer, "Assets/wall.png");
	assets->font = TTF_OpenFont("Assets/m5x7.ttf", 16);
	if(!(assets->texture && assets->appletex &&
		 assets->snakehead && assets->grass &&
		 assets->tail && assets->wall))
	{
		SDL_Log("Resources cannot be loaded.\nError: %s\n", SDL_GetError());
		return false;
	}
	if(!assets->font)
	{
		printf("Resources cannot be loaded.\nError: %s\n", TTF_GetError());
		return false;
	}
	return true;
}


void quitSDL(contextObject* assets)
{
	SDL_DestroyTexture(assets->snakehead);
	SDL_DestroyTexture(assets->appletex);
	SDL_DestroyTexture(assets->wall);
	SDL_DestroyTexture(assets->tail);
	SDL_DestroyTexture(assets->grass);
	SDL_DestroyTexture(assets->texture);
	TTF_CloseFont(assets->font);
	
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    window = NULL;
    renderer = NULL;
	TTF_Quit();
    SDL_Quit();
}
