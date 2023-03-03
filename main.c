#include <stdio.h>
#include "main.h"
#include <stdbool.h>
#include "types.h"
#include <math.h>
#include <sys/time.h>

SDL_Texture* texture;
SDL_Texture* background_texture;

int init(SDL_Window **window, SDL_Renderer **renderer)
{
    SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    // Create an application window with the following settings:
    *window = SDL_CreateWindow(
        "Wolfenstein 3D",       // window title
        SDL_WINDOWPOS_CENTERED, // initial x position
        SDL_WINDOWPOS_CENTERED, // initial y position
        WIDTH,                  // width, in pixels
        HEIGHT,                 // height, in pixels
        0                       // flags - see below
    );
    // Check that the window was successfully made
    if (*window == NULL)
    {
        // In the event that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    *renderer = SDL_CreateRenderer(*window, -1, 0);
    if (*renderer == NULL)
    {
        // In the event that the renderer could not be made...
        printf("Could not create renderer: %s\n", SDL_GetError());
        return 2;
    }
    return 0;
}
void loadTextures(SDL_Renderer * renderer){
    SDL_Surface* image = IMG_Load("wall.png");
    SDL_Surface* background = IMG_Load("background.jpg");
    texture = SDL_CreateTextureFromSurface(renderer, image);
    background_texture = SDL_CreateTextureFromSurface(renderer, background);
    free(image);
    free(background);
}

void drawgame(SDL_Renderer *renderer, Player *player)
{
    SDL_RenderClear(renderer);
    // Paint it black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, NULL);
    
    drawMap(renderer);
    drawPlayer(renderer, player);
    SDL_RenderPresent(renderer);
}

int gameloop(SDL_Window *window, SDL_Renderer *renderer)
{

    Uint64 last_time, current_time;
    last_time = SDL_GetTicks64();
    double elapsed_time;
    int game_running = 1;
    Player player = {210, 210, 0};
    player.i = xpos(player.x);
    player.j = xpos(player.y);
    player.air = false;

    while (game_running)
    {
        drawgame(renderer, &player);
        SDL_Event event;

        const Uint8 *KEYS = SDL_GetKeyboardState(NULL);
        while (SDL_PollEvent(&event))
        {
            char keypressed;
            switch (event.type)
            {
            case SDL_QUIT:
                game_running = 0;
                break;

            case SDL_KEYDOWN:
                keypressed = event.key.keysym.sym;
                if (keypressed == QUITKEY)
                {
                    game_running = 0;
                    break;
                }
                break;
            }
        }
        current_time = SDL_GetTicks64();
        double elapsed_time = (current_time - last_time) / 1000.0;
        // in seconds

        last_time = current_time;
        SDL_Delay(22);
        player_update(KEYS, &player, elapsed_time);
    }
    return 0;
}

int main()
{
    SDL_Window *window; // Declare a pointer
    SDL_Renderer *renderer;
    int r;
    if ((r = init(&window, &renderer)))
        return r;


    loadTextures(renderer);
    gameloop(window, renderer);

    SDL_Quit();
}