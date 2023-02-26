#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct{ 
    double x;
    double y;
    int i;
    int j;
    double angle;
} Player;

void drawMap(SDL_Renderer * renderer);

void drawPlayer(SDL_Renderer *renderer, Player *player);

void player_update(const Uint8 * keys, Player * player, double delta_time);