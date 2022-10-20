//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <memory>

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
    
SDL_Renderer* gRenderer = NULL;

//Screen dimension constants
const int SCREEN_WIDTH = 640*2;
const int SCREEN_HEIGHT = 480*2;

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

enum directions 
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Position
{
    int x;
    int y;
};

class Player
{
    private:
        Position pos;
        int speed = 1;
        int defaultSize = 40;
        int size = 40;
        
    public:
        SDL_Texture* pTexture = NULL;
        Player();
        Player(int x, int y);
        ~Player();
        void move(directions dir);
        void draw();
        void free();
        void grow() 
        {
            size += 1;
        }
        void resetSize()
        {
            size = defaultSize;
        }
        Position getPos() {return pos;};
        int getSize() {return size;}
};

class Goal
{
    private:
        Position pos;
        int size = 100;
        SDL_Texture *gTexture = NULL;
    public:
        Goal();
        Goal(Position playerPos, int playerSize, int worldWidth, int worldHeight, int worldBorder);
        ~Goal();
        void draw();
        void free();
        Position getPos() {return pos;};
        int getSize() {return size;}
        move(Position playerPos, int playerSize, int worldWidth, int worldHeight, int worldBorder);
};

class World
{
    private:
        Player player;
        Goal goal;
        int borderSize = 30;
        int width;
        int height;
    public:
        World(int width, int height) : 
            width(width), 
            height(height), 
            player(width/2, height/2), 
            goal(player.getPos(), player.getSize(), width, height, borderSize) 
            {};
        ~World();
        spawnGoal();
        draw();
        takeInput();
        bool checkCollisions();
        mainLoop();

};

Player::Player(int x, int y) 
{
    pos.x = x;
    pos.y = y;

    //Loading success flag
    pTexture = IMG_LoadTexture(gRenderer, "jonas.png");
    if( pTexture == NULL )
    {
        printf( "Failed to load player texture image!\n" );
    }
}

Player::~Player()
{
	//Deallocate
	free();
}

void Player::move(directions dir)
{
    switch (dir)
        {
        case UP:
            pos.y -= speed;
            break;
        case DOWN:
            pos.y += speed;
            break;
        case LEFT:
            pos.x -= speed;
            break;
        case RIGHT:
            pos.x += speed;
            break;
        default:
            break;
        }
}

void Player::draw()
{
    //Set rendering space and render to screen
	SDL_Rect renderQuad = { pos.x -size/2, pos.y-size/2, size, size };

	//Render to screen
	SDL_RenderCopy( gRenderer, pTexture, NULL, &renderQuad );
}

void Player::free()
{
    if( pTexture != NULL )
	{
		SDL_DestroyTexture( pTexture );
		pTexture = NULL;
	}
}


Goal::Goal(Position playerPos, int playerSize, int worldWidth, int worldHeight, int worldBorder)
{
    move(playerPos, playerSize, worldWidth, worldHeight, worldBorder);

    gTexture = IMG_LoadTexture(gRenderer, "store.png");
    if( gTexture == NULL )
    {
        printf( "Failed to load goal texture image!\n" );
    }
}

Goal::~Goal()
{
	//Deallocate
	free();
}

void Goal::draw()
{
    //Set rendering space and render to screen
	SDL_Rect renderQuad = { pos.x -size/2, pos.y-size/2, size, size };

	//Render to screen
	SDL_RenderCopy( gRenderer, gTexture, NULL, &renderQuad );
}

Goal::move(Position playerPos, int playerSize, int worldWidth, int worldHeight, int worldBorder)
{
    do
    {
        pos.x = rand() % (worldWidth - 2*worldBorder - size) + size/2 + worldBorder;
        pos.y = rand() % (worldHeight - 2*worldBorder- size) + size/2 + worldBorder;
    } while (abs(pos.x-playerPos.x) < playerSize*8 || abs(pos.y-playerPos.x) < playerSize*8 );
    printf("%d, %d\n", pos.x, pos.y);
}

void Goal::free()
{
    if( gTexture != NULL )
	{
		SDL_DestroyTexture( gTexture );
		gTexture = NULL;
	}
}


World::draw()
{
    SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0);
    SDL_RenderClear(gRenderer);

    SDL_Rect outlineRect = { borderSize, borderSize, width-borderSize*2, height-borderSize*2 };
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );        
    SDL_RenderDrawRect( gRenderer, &outlineRect );

    player.draw();
    goal.draw();

    //Update screen
    SDL_RenderPresent( gRenderer );
    SDL_Delay(1);
}

World::takeInput()
{
    bool quit = false;
    SDL_Event e;
    const Uint8 *keystate;

    while (SDL_PollEvent(&e) != 0) 
    {
        if( e.type == SDL_QUIT)
        {
            quit = true;
        }
    }

    keystate = SDL_GetKeyboardState(NULL);
    // const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_LEFT])
    {
        player.move(LEFT);
    }
    if (keystate[SDL_SCANCODE_RIGHT])
    {
        player.move(RIGHT);
    }
    if (keystate[SDL_SCANCODE_UP])
    {
        player.move(UP);
    }
    if (keystate[SDL_SCANCODE_DOWN])
    {
        player.move(DOWN);
    }
    if (keystate[SDL_SCANCODE_SPACE])
    {
        player.grow();
    }
    else
    {
        player.resetSize();
    }
    if (keystate[SDL_SCANCODE_Q])
    {
        quit = true;
    }
    
    return quit;
}

bool World::checkCollisions()
{
    Position playerPos = player.getPos();
    int playerSize = player.getSize();

    Position goalPos = goal.getPos();
    int goalSize = goal.getSize();
    // Walls
    if (playerPos.y <= borderSize + playerSize/2 || playerPos.y >= height - borderSize*2 + playerSize/2)
    {
        return true;
    }
    if (playerPos.x <= borderSize + playerSize/2 || playerPos.x >= width - borderSize*2 + playerSize/2)
    {
        return true;
    }
    if 
    (
        abs(playerPos.x - goalPos.x) <= (goalSize + playerSize)/2 && 
        abs(playerPos.y - goalPos.y) <= (goalSize + playerSize)/2)
    {
        goal.move(playerPos, playerSize, width, height, borderSize);
    }
    return false;
}

World::mainLoop()
{   
    bool quit = false;
    bool dead = false;
    while (!quit && !dead)
    {         
        draw();
        quit = takeInput();
        dead = checkCollisions();
    }
}

World::~World()
{
    player.free();
    goal.free();
}

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

SDL_Texture* loadTexture(std::string path);

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
        gWindow = SDL_CreateWindow( "Epic game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

void close()
{

    //Destroy window    
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void draw()
{

}

bool input(Player player)
{   
    bool quit = false;
    SDL_Event e;
    const Uint8 *keystate;

    while (SDL_PollEvent(&e) != 0) 
    {
        if( e.type == SDL_QUIT)
        {
            quit = true;
        }
    }

    keystate = SDL_GetKeyboardState(NULL);
    // const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_LEFT])
    {
        player.move(LEFT);
    }
    if (keystate[SDL_SCANCODE_RIGHT])
    {
        player.move(RIGHT);
    }
    if (keystate[SDL_SCANCODE_UP])
    {
        player.move(UP);
    }
    if (keystate[SDL_SCANCODE_DOWN])
    {
        player.move(DOWN);
    }
    if (keystate[SDL_SCANCODE_SPACE])
    {
        player.grow();
    }
    else
    {
        player.resetSize();
    }
    if (keystate[SDL_SCANCODE_Q])
    {
        quit = true;
    }
    
    return quit;
}

int main(int argc, char *args[])
{
    if( !init())
    {   
        printf( "Failed to initalize\n");
    }
    else
    {

        bool quit = false;
        SDL_Event e;

        World world(SCREEN_WIDTH, SCREEN_HEIGHT);

        world.mainLoop();
            
    }

    close();
    
    return 0;
}
