#include "types.h"
#include "main.h"
#include <math.h>

#define PI 3.14159265359
#define TAO (2 * PI)
#define SCREEN_DIST (((double)WIDTH) / 2.0) / tan(FOV / 2.0)

extern int field[GAME_HEIGHT][GAME_WIDTH];

int amount = 0;
extern SDL_Texture *texture;
extern SDL_Texture *background_texture;

void drawPlayer(SDL_Renderer *renderer, Player *player)
{
    // Draw the vision
    // how much dy till vertical-bars
    // checks direction
    double center_angle = player->angle;
    double current_bar = 0;
    double half_fov_tan = tan(FOV / 2);
    SDL_SetRenderDrawColor(renderer, FLOOR);
    SDL_Rect bottom_half;
    bottom_half.h = HEIGHT / 2;
    bottom_half.y = HEIGHT / 2;
    bottom_half.x = 0;
    bottom_half.w = WIDTH;
    SDL_RenderFillRect(renderer, &bottom_half);

    // background
    SDL_Rect background_portion_1;
    SDL_Rect background_res;
    background_res.x = 0;
    background_res.h = HEIGHT / 2;
    background_res.y = 0;
    background_res.w = WIDTH;

    background_portion_1.h = BACKGROUND_HEIGHT;
    background_portion_1.y = 0;

    int w = ((FOV) / TAO) * BACKGROUND_WIDTH;
    const int W = w;

    int x = ((TAO - player->angle) / TAO) * BACKGROUND_WIDTH;

    int right_edge = x + w;

    background_portion_1.x = x;
    background_portion_1.w = w;

    if (right_edge > BACKGROUND_WIDTH)
    {
        int second_w = right_edge % BACKGROUND_WIDTH;
        w -= second_w;
        double prop = ((double)w / (double)(w + second_w));
        background_res.w = WIDTH * prop;
        SDL_Rect background_res_2 = background_res;
        int _w = WIDTH * (1 - prop);
        background_res_2.x = background_res.w;
        background_res_2.w = _w;

        SDL_Rect background_portion_2 = background_portion_1;
        background_portion_2.x = 0;
        background_portion_2.w = second_w;

        background_portion_1.w = w;

        SDL_RenderCopy(renderer, background_texture, &background_portion_2, &background_res_2);
    }
    SDL_RenderCopy(renderer, background_texture, &background_portion_1, &background_res);

    for (double current_angle = center_angle + (FOV / 2); current_angle > (center_angle - FOV / 2); current_angle -= STEP, current_bar += BARSTEP)
    {
        int int_dx_to_intersection;
        double dx_to_intersection, dy_to_intersection;
        double x_vertical, y_vertical;
        double length_to_vert;
        int direction_vector = 0;
        int rounded_x = (int)player->x;
        double x_fraction = player->x - rounded_x;
        int_dx_to_intersection = rounded_x % diff_x;
        double vertical_cos = cos(current_angle);

        if (vertical_cos < 0)
        {
            direction_vector = -1;
            // Int dx not accounting for direction
            x_vertical = rounded_x - int_dx_to_intersection;
            // Double dx does!
            dx_to_intersection = -(int_dx_to_intersection + x_fraction);
        }

        else
        {
            direction_vector = 1;
            int_dx_to_intersection = diff_x - int_dx_to_intersection;
            x_vertical = rounded_x + int_dx_to_intersection;
            // Now in double
            dx_to_intersection = int_dx_to_intersection - x_fraction;
        }

        double slope = tan(current_angle);
        dy_to_intersection = slope * dx_to_intersection;
        // Minus because inverted y
        y_vertical = player->y + (-dy_to_intersection);
        double dy = -(slope * (diff_x)*direction_vector);
        // Find wall collision  JEEZZZ
        int i = player->i + direction_vector;
        int _j;
        while (((_j = ypos(y_vertical)) < GAME_HEIGHT) && _j >= 0 && !field[_j][i])
        {
            x_vertical += direction_vector * diff_x;
            y_vertical += dy;
            i += direction_vector;
        }
        double total_dx = player->x - x_vertical;
        length_to_vert = fabs((1 / vertical_cos) * total_dx);

        int int_dy_to_intersection;
        double x_horizontal, y_horizontal;
        double length_to_hor;
        int rounded_y = (int)player->y;
        double y_fraction = player->y - rounded_y;
        int_dy_to_intersection = rounded_y % diff_y;

        double horizontal_sin = sin(current_angle);
        if (horizontal_sin < 0)
        {
            direction_vector = 1;
            int_dy_to_intersection = diff_y - int_dy_to_intersection;
            y_horizontal = rounded_y + int_dy_to_intersection;
            // Now in double
            dy_to_intersection = int_dy_to_intersection - y_fraction;
        }
        else
        {
            direction_vector = -1;
            y_horizontal = rounded_y - int_dy_to_intersection;
            // REMEMBER UP IS NEGATIVE DIRECTION
            dy_to_intersection = -(int_dy_to_intersection + y_fraction);
        }

        dx_to_intersection = (1 / slope) * (-dy_to_intersection);
        x_horizontal = player->x + dx_to_intersection;
        double dx = (1 / slope) * (diff_y) * (-1 * direction_vector);
        // Find wall collision  JEEZZZ
        int j = player->j + direction_vector;
        int _i;
        while ((_i = xpos(x_horizontal)) < GAME_WIDTH && _i >= 0 && !field[j][_i])
        {
            y_horizontal += direction_vector * diff_y;
            x_horizontal += dx;
            j += direction_vector;
        }
        double total_dy = player->y - y_horizontal;
        length_to_hor = fabs((1 / horizontal_sin) * total_dy);

        double length, x_end, y_end;
        int offset;
        if (length_to_hor < length_to_vert)
        {
            length = length_to_hor;
            x_end = x_horizontal;
            y_end = y_horizontal;
            int _x = x_end;
            double frac = x_end - _x;
            if (horizontal_sin > 0)
            {
                offset = (_x % diff_x);
            }
            else
            {
                offset = diff_x - (_x % diff_x);
            }
        }
        else
        {
            length = length_to_vert;
            x_end = x_vertical;
            y_end = y_vertical;
            int _y = y_end;
            if (vertical_cos > 0)
            {
                offset = (_y % diff_y);
            }
            else
            {
                offset = diff_y - (_y % diff_y);
            }
        }

        if (DIMENSIONS == 2)
        {
            SDL_FRect player_rect = {player->x, player->y, 10, 10};

            SDL_RenderFillRectF(renderer, &player_rect);

            SDL_SetRenderDrawColor(renderer, 51, 255, 255, 255);

            SDL_RenderDrawLineF(renderer, player->x, player->y, x_end, y_end);
        }
        else
        {
            SDL_FRect bar;
            bar.x = current_bar;
            bar.w = BARSTEP;
            double length_from_eye = fabs(cos(center_angle - current_angle) * length);
            bar.h = ((SCREEN_DIST) / length_from_eye) * diff_y;
            bar.y = (((HEIGHT - bar.h)) / 2) + atan(player->airpos / length_from_eye) * 200;
            SDL_Rect src;
            src.x = offset * (1000 / diff_x);
            src.h = 1000;
            src.y = 0;
            src.w = (1000 / diff_x) * (half_fov_tan * length_from_eye) / WIDTH;
            // src.w = 1;

            SDL_RenderCopyF(renderer, texture, &src, &bar);
            // SDL_RenderFillRectF(renderer, &bar);
        }
    }
}

void check_with_walls(double dx, double dy, Player *player)
{
    double x = player->x;
    double y = player->y;
    int adjusted_x = xpos(x + dx);
    int adjusted_y = ypos(y + dy);

    if (!(field[ypos(y)][adjusted_x]))
    {
        player->x = x + dx;
        player->i = adjusted_x;
    }
    if (!(field[adjusted_y][xpos(x)]))
    {
        player->y = y + dy;
        player->j = adjusted_y;
    }
}

void player_update(const Uint8 *keys, Player *player, double delta_time)
{
    double speed = 200.0 * delta_time;
    double dy = 0.0;
    double dx = 0.0;
    if (keys[SDL_SCANCODE_W])
    {
        dy += sin(player->angle) * speed;
        dx += cos(player->angle) * speed;
    }
    if (keys[SDL_SCANCODE_S])
    {
        dy -= (sin(player->angle) * speed);
        dx -= (cos(player->angle) * speed);
    }
    if (keys[SDL_SCANCODE_D])
    {
        dy -= (cos(player->angle) * speed);
        dx += (sin(player->angle) * speed);
    }
    if (keys[SDL_SCANCODE_A])
    {
        dy += (cos(player->angle) * speed);
        dx -= (sin(player->angle) * speed);
    }

    if (keys[SDL_SCANCODE_LEFT])
    {
        player->angle += 1.5 * delta_time;
    }
    if (keys[SDL_SCANCODE_RIGHT])
    {
        player->angle -= 1.5 * delta_time;
    }

    if (!player->air)
    {
        if (keys[SDL_SCANCODE_SPACE])
        {
            printf("hi");
            player->air = true;
            player->airpos = 0;
            player->jump_time = 0;
        }
    }
    // In air
    else
    {
        player->jump_time += delta_time;
        int airpos = (HEIGHT / 10) * sin(TAO * player->jump_time);
        printf("%d", airpos);
        if (airpos < 0)
        {
            player->airpos = 0;
            player->jump_time = 0;
            player->air = false;
        }
        else
        {
            player->airpos = airpos;
        }
    }
    // Inverted y
    if (dy != 0.0 || dx != 0.0)
    {
        dy = -dy;
        check_with_walls(dx, dy, player);
    }
    player->angle = fmod(player->angle, TAO);
    if (player->angle < 0)
    {
        player->angle = TAO - player->angle;
    }
}