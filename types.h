#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct
{
    double x;
    double y;
    int i;
    int j;
    double angle;
    bool air;
    int airpos;
    double jump_time;
} Player;

void drawMap(SDL_Renderer *renderer);

void drawPlayer(SDL_Renderer *renderer, Player *player);

void player_update(const Uint8 *keys, Player *player, double delta_time);