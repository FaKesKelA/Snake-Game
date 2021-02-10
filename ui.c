#include "ui.h"

//Main menu state where the user can choose which map to play on
void inMainMenu(state* currentState, int* mapType, Tile** Grid, int columns, int rows, SDL_Renderer* renderer, TTF_Font* font)
{
    // creating the buttons and placing them in the Grid
    button quitButton = buttonCreator(Grid[(rows/2)+3][(columns/2)-2].location, 4, 2, "Quit", 3);
    button mapClassicButton = buttonCreator(Grid[(rows/2)-1][(columns/2)-2].location, 4, 2, "Classic Map", 3);
    button mapHardButton = buttonCreator(Grid[(rows/2)+1][(columns/2)-2].location, 4, 2, "Hard Map", 3);
    
    while(true)
    {
        SDL_Event event;
        SDL_WaitEvent(&event);
        if(event.type == SDL_QUIT)
        {
            *currentState = quit;
            return;
        }
        SDL_RenderClear(renderer);
        textRenderer(renderer, font, &quitButton.text);
        textRenderer(renderer, font, &mapClassicButton.text);
        textRenderer(renderer, font, &mapHardButton.text);
        
        // Button that quits the program when pressed
        buttonAction(&quitButton, &event);
        if (quitButton.clicked)
        {
            quitButton.clicked = false;
            *currentState = quit;
            return;
        }
        // Buttons that start the map when clicked
        buttonAction(&mapClassicButton, &event);
        if (mapClassicButton.clicked)
        {
            mapClassicButton.clicked = false;
            *currentState = loading;
            *mapType = 0;
            return;
        }
        buttonAction(&mapHardButton, &event);
        if (mapHardButton.clicked)
        {
            mapHardButton.clicked = false;
            *currentState = loading;
            *mapType = 1;
            return;
        }
        
        SDL_RenderPresent(renderer);
    }
    
}
// Pressing the button, actions
//https://stackoverflow.com/questions/39133873/how-to-set-a-gui-button-in-the-win32-window-using-sdl-c
static void buttonAction(button *btn, SDL_Event *ev)
{
    if(ev->type == SDL_MOUSEBUTTONDOWN)
    {
        if(ev->button.button == SDL_BUTTON_LEFT &&
                ev->button.x >= btn->position.x &&
                ev->button.x <= (btn->position.x + btn->position.w) &&
                ev->button.y >= btn->position.y &&
                ev->button.y <= (btn->position.y + btn->position.h))
        {
            btn->clicked = true;
            btn->text.color = btn->color[2];
        }
    }
    else
    {
        if(ev->button.x >= btn->position.x && ev->button.x <= (btn->position.x + btn->position.w) &&
           ev->button.y >= btn->position.y && ev->button.y <= (btn->position.y + btn->position.h))
            btn->text.color = btn->color[1];
        else
            btn->text.color = btn->color[0];
    }
}

// Creates the buttons on the screen placing them inside the grid
static button buttonCreator(SDL_Rect gridPosition, int width, int height, char* text, int scaleSize)
{
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color blue = {40, 204, 223, 255};
    SDL_Color darkBlue = {57, 120, 168, 255};
    
    button tempButton;
            
    SDL_Rect temPosition = gridPosition;
    temPosition.w *= width;
    temPosition.h *= height;
    tempButton.position = temPosition;
    tempButton.clicked = false;
    tempButton.color[0] = white;
    tempButton.color[1] = blue;
    tempButton.color[2] = darkBlue;
    tempButton.text = textCreator(tempButton.position, text, scaleSize, white);
    return tempButton;
}

textStruct textCreator(SDL_Rect box, char self[16], int scale, SDL_Color color)
{
    textStruct tempText;
    tempText.scale = scale;
    strcpy(tempText.self, self);
    tempText.box = box;
    tempText.color = color;
    
    return tempText;
}

// puts the text on the screen
void textRenderer(SDL_Renderer* renderer, TTF_Font* font, textStruct* word)
{
    SDL_Surface* tempSurface = TTF_RenderText_Solid(font, word->self, word->color);
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    
    SDL_Rect location = word->box;
    TTF_SizeText(font, word->self, &word->box.w, &word->box.h);
    word->box.w *= word->scale;
    word->box.h *= word->scale;
    word->box.x += (location.w - word->box.w) / 2;
    word->box.y += (location.h - word->box.h) / 2;
    
    SDL_RenderCopy(renderer, tempTexture, NULL, &word->box);
    SDL_FreeSurface(tempSurface);
    SDL_DestroyTexture(tempTexture);
}

void deadScreen(Tile** Grid, int columns, int rows, state* currentState, int score, SDL_Renderer* renderer, TTF_Font* font)
{
    button quitButton = buttonCreator(Grid[0][0].location, 2, 1, "Quit", 3);
    SDL_Rect box = Grid[(rows/2)-1][(columns/2)-1].location;
    box.w *= 2;
    SDL_Color red = {230, 72, 46, 255};
    char scoreText[32] = "";
    sprintf(scoreText, "Final score: %02d", score);
    textStruct deadText = textCreator(box, "Game Over", 8, red);
    box.y += box.h *2;
    red.r = 190;
    textStruct hint = textCreator(box, "Press any button to continue..", 3, red);
    box.y = 0;
    textStruct finalScore = textCreator(box, scoreText, 3, red);
    
    while(true)
    {
        SDL_RenderClear(renderer);
        textRenderer(renderer, font, &quitButton.text);
        textRenderer(renderer, font, &deadText);
        textRenderer(renderer, font, &hint);
        textRenderer(renderer, font, &finalScore);
        SDL_RenderPresent(renderer);
        SDL_Event event;
        SDL_WaitEvent(&event);
        if(event.type == SDL_QUIT)
        {
            *currentState = quit;
            return;
        }
        else if(event.type == SDL_KEYDOWN)
        {
            *currentState = mainMenu;
            return;
        }
        buttonAction(&quitButton, &event);
        if (quitButton.clicked)
        {
            quitButton.clicked = false;
            *currentState = quit;
            return;
        }
    }
    
}

