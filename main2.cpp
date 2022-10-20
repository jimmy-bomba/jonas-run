//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

//Key press surfaces constants
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

// class pos
// {
//     public:
//         int x;
//         int y;
//         int stepSize = 10;
//         void moveUp() {y -= stepSize;};
//         void moveDown() {y += stepSize;};
//         void moveRight() {x += stepSize;};
//         void moveLeft() {x -= stepSize;};
// };
enum directions 
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};
class pos
{
    public:
        int x;
        int y;
        int stepSize = 1;
        directions dir;
        void move(); 
};

void pos::move() 
{
    switch (dir)
        {
        case UP:
            y -= stepSize;
            break;
        case DOWN:
            y += stepSize;
            break;
        case LEFT:
            x -= stepSize;
            break;
        case RIGHT:
            x += stepSize;
            break;
        default:
            break;
        }
}


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

SDL_Texture* loadTexture(std::string path);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
    
SDL_Renderer* gRenderer = NULL;

SDL_Texture* gTexture = NULL;
SDL_Texture* tTexture = NULL;

//Screen dimension constants
const int SCREEN_WIDTH = 640*2;
const int SCREEN_HEIGHT = 480*2;

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
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL)
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor( gRenderer, 0xF8, 0xF1, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init( imgFlags) & imgFlags))
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
                
            }
            
        }
    }
    return success;    
}

SDL_Texture* loadTexture( std::string path)
{
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load( path.c_str());
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface);
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        SDL_FreeSurface( loadedSurface);
    }
    return newTexture;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;
    return success;
}

void close()
{
    //Free loaded image
    SDL_DestroyTexture( gTexture );
    gTexture = NULL;

    //Destroy window    
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
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

            pos playerPos;
            playerPos.x = SCREEN_WIDTH/2;
            playerPos.y = SCREEN_HEIGHT/2;

            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if( e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    else if ( e.type == SDL_KEYDOWN)
                    {
                        switch( e.key.keysym.sym)
                        {
                            case SDLK_UP:
                            playerPos.dir = UP;
                            break;
                            case SDLK_DOWN:
                            playerPos.dir = DOWN;
                            break;
                            case SDLK_LEFT:
                            playerPos.dir = LEFT;
                            break;
                            case SDLK_RIGHT:
                            playerPos.dir = RIGHT;
                            break;
                            break;

                        }
                    }
                }
                SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0);
                SDL_RenderClear(gRenderer);

                playerPos.move();

                SDL_Rect fillRect = {playerPos.x, playerPos.y, SCREEN_WIDTH/32, SCREEN_WIDTH/32};
                SDL_SetRenderDrawColor(gRenderer,0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderFillRect(gRenderer, &fillRect);

                SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );        
                SDL_RenderDrawRect( gRenderer, &outlineRect );

                //Draw blue horizontal line
                // SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );        
                // SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 );

                //Draw vertical line of yellow dots
                // SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
                // for( int i = 0; i < SCREEN_HEIGHT; i += 4 )
                // {
                //     SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
                // }

                //Update screen
                SDL_RenderPresent( gRenderer );
                // SDL_Delay(10);
            }
                
        }
        
    }
    close();
    
    return 0;
}
