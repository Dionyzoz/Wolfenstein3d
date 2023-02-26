#include "types.h"
#include "main.h"
#include <math.h>

#define SCREEN_DIST (((double)WIDTH) / 2.0) / tan(FOV / 2.0)

extern int field[GAME_HEIGHT][GAME_WIDTH];

int amount = 0;

void drawPlayer(SDL_Renderer *renderer, Player *player)
{
    // Draw the vision
    // how much dy till vertical-bars
    // checks direction
    double center_angle = player->angle;
    double current_bar = 0;
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
        if (length_to_hor < length_to_vert)
        {
            length = length_to_hor;
            x_end = x_horizontal;
            y_end = y_horizontal;
        }
        else
        {
            length = length_to_vert;
            x_end = x_vertical;
            y_end = y_horizontal;
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
            double trigprop = fabs(cos(center_angle - current_angle) * length);
            bar.h = ((SCREEN_DIST) / trigprop) * diff_x;
            double x;
            int color;
            if (HEIGHT < bar.h)
            {
                color = 255;
            }
            else
            {
                color = 255 - pow((((HEIGHT) / bar.h) - 1), 1.6) ;
            }
            if (color < 0)
            {
                color = 0;
            }
            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            bar.y = (HEIGHT - bar.h) / 2;

            SDL_RenderFillRectF(renderer, &bar);
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
    double speed = 100.0 * delta_time;
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
        player->angle += 1.5  * delta_time;
    }
    if (keys[SDL_SCANCODE_RIGHT])
    {
        player->angle -= 1.5 * delta_time;
    }
    // Inverted y
    if (dy != 0.0 || dx != 0.0)
    {
        dy = -dy;
        check_with_walls(dx, dy, player);
    }
}