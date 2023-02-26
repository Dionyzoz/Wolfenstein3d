#include <SDL2/SDL.h>

int main() {
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create a window and renderer
    SDL_Window* window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // // Clear the renderer
    // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // SDL_RenderClear(renderer);

    // // Draw a rectangle in the center of the window
    // SDL_Rect rect = { 240, 160, 160, 160 };
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // SDL_RenderDrawRect(renderer, &rect);

    // // Present the renderer
    // SDL_RenderPresent(renderer);

    // // Wait for a key press
    SDL_Event event;
    while (SDL_WaitEvent(&event) && event.type != SDL_QUIT && event.type != SDL_KEYDOWN) {}

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}







