#include <SDL2/SDL.h>
#include <stdio.h>
#include <chrono>

#include "drawLines.hpp"
#include "image.hpp"
#include "vector.hpp"
#include "pipeline.hpp"

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


    //test points
    Vec2i p0 = Vec2i(320, 50);
    Vec2i p1 = Vec2i(20, 400);
    Vec2i p2 = Vec2i(620, 400);
    Vec2i p3 = Vec2i(200, 100);

    Vec2i p0a = Vec2i(320, 100);
    Vec2i p1a = Vec2i(320, 300);
    Vec2i p2a = Vec2i(430, 200);


    bool run = true;
    int i = 0;
    while(run) {
    auto exec_start = std::chrono::high_resolution_clock::now();
    pipeline();
    drawLines(p0, p1, &image);
    drawLines(p1, p2, &image);
    drawLines(p2, p0, &image);
   
    drawLines(p0a, p1a, &image);
    drawLines(p1a, p2a, &image);
    drawLines(p2a, p0a, &image);

        SDL_UpdateTexture(texture, NULL, image.pixels, SCREEN_WIDTH * sizeof(Uint32));

        if(SDL_PollEvent(&event) && event.type == SDL_QUIT){
            break;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    auto exec_end = std::chrono::high_resolution_clock::now();
    printf("Loop time:%i ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(exec_end - exec_start).count());

    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}










