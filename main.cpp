#include <SDL2/SDL.h>
#include <stdio.h>
#include "drawLines.hpp"
#include "image.hpp"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main(void) {
    SDL_Event event;
    SDL_Window* window;
    SDL_Surface* surface;

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("SDL test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    surface = SDL_GetWindowSurface(window);

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture * texture = SDL_CreateTexture(renderer,
            SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    Image image = Image(SCREEN_WIDTH, SCREEN_HEIGHT);


    SDL_LockSurface(surface); 

    //EVERYTHING ELSE GOES HERE
    drawLines(&image);
    //

    SDL_UnlockSurface(surface);

    bool run = true;
    int i = 0;
    while(run) {
        SDL_UpdateTexture(texture, NULL, image.pixels, SCREEN_WIDTH * sizeof(Uint32));

        if(SDL_PollEvent(&event) && event.type == SDL_QUIT){
            break;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}










