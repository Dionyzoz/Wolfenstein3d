#include <stdio.h>
#include "types.h"
#include "main.h"

int field[GAME_HEIGHT][GAME_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

void drawMap(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    if (DIMENSIONS == 2)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int i = 0; i < GAME_HEIGHT; i++)
        {
            for (int j = 0; j < GAME_WIDTH; j++)
            {
                if (field[i][j])
                {
                    SDL_Rect rect = {j * diff_x, i * diff_y, diff_x, diff_y};
                    ;
                    SDL_RenderDrawRect(renderer, &rect);
                }
            }
        }
    }
}