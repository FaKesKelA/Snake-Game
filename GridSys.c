#include "GridSys.h"

// 2 dimensional array that creates the layout of the map
Tile** Area(SDL_Rect sqr, int rows, int columns, Tile** Grid)
{
    int y, x;
    if(rows == 0 || columns ==0)
        return NULL;
    
    Grid = (Tile**)malloc(rows * sizeof(Tile*));
    Grid[0] = (Tile*)malloc(rows * columns*sizeof(Tile));
    
    for(y = 1; y < rows; y++)
    {
        Grid[y] = Grid[0] + y * columns;
    }
    if(Grid[0] == NULL)
    {
        return NULL;
    
    }
    
    for(y = 0; y < rows; y++)
    {
        for(x = 0; x < columns; x++)
        {
            Grid[y][x].collision = 0;
            Grid[y][x].hasApple = 0;
            Grid[y][x].hasSnake = 0;
            Grid[y][x].location.x = sqr.x + x *(sqr.w / columns);
            Grid[y][x].location.y = sqr.y + y *(sqr.h / rows);
            Grid[y][x].location.w = sqr.w / columns;
            Grid[y][x].location.h = sqr.h / rows;
        }
    }
    return Grid;
}

// free function
void freeArea(Tile** Grid)
{
    free(Grid[0]);
    free(Grid);
}
// this function reads in the maps for the user depending on what they chose in the main menu
void loadMap(Tile** Grid, const int rows, const int columns,int mapNumber)
{
    char* mapPath = "Assets/map.txt";
    char* emptyMapPath = "Assets/empty map.txt";
    FILE* map = NULL;
    char line[columns+2];
    int x, y;
    
    if(mapNumber)
        map = fopen(mapPath, "r");
    else
        map = fopen(emptyMapPath, "r");
    
    for(y = 0; y < rows; y++)
    {
        fgets(line, columns+2, map);
        line[columns] = '\0';
     
        for(x = 0; x < columns; x++)
        {
            Grid[y][x].hasSnake = 0;
            Grid[y][x].hasApple = 0;
            Grid[y][x].collision = (int)line[x] -48;
        }
    }
    fclose(map);
}

