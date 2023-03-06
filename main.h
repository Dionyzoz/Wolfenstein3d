#define GAME_HEIGHT 9
#define GAME_WIDTH 13
#define diff_x 60
#define diff_y 60
#define HEIGHT GAME_HEIGHT *diff_y
#define WIDTH GAME_WIDTH *diff_x
#define HALFWIDTH WIDTH / 2.0
#define DELAY 3000
#define QUITKEY SDLK_ESCAPE
#define DIMENSIONS 3
#define FOV 3.143 / 3.0
#define RAYS (6 * WIDTH)

#define BARSTEP (1.0 / 6.0)
#define STEP (FOV / (double)RAYS)

#define xpos(x) (int)((x) / diff_x)
#define ypos(y) (int)((y) / diff_y)

#define BACKGROUND_WIDTH 2560
#define BACKGROUND_HEIGHT 1024

#define FLOOR 165, 42, 42, 255