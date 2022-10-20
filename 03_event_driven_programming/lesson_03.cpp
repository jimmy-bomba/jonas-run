//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
    
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gXOut = NULL;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init()
{
    bool success = true;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            gScreenSurface = SDL_GetWindowSurface( gWindow);
        }
    }
    return success;    
}

bool loadMedia() 
{
    bool success = true;

    gXOut = SDL_LoadBMP( "x.bmp");
    if (gXOut == NULL)
    {
        printf( "Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError() );
        success = false;
    }

    return success;
}

void close() 
{
    SDL_FreeSurface( gXOut);
    gXOut = NULL;

    SDL_DestroyWindow( gWindow);
    gWindow = NULL;

    SDL_Quit();
}

int main(int argc, char *args[])
{
    if( !init())
    {   
        printf( "Failed to initalize\n");
    }
    else
    {
        if( !loadMedia())
        {
            printf("Falied meda\n");
        }
        else
        {
            bool quit = false;
            SDL_Event e;
            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if( e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                }
                SDL_BlitSurface( gXOut, NULL, gScreenSurface, NULL);

                SDL_UpdateWindowSurface( gWindow);
                
            }
            
            SDL_BlitSurface(gXOut, NULL, gScreenSurface, NULL);
            SDL_UpdateWindowSurface( gWindow);
            SDL_Delay(2000);
        }
        
    }
    close();
    
    return 0;
}
