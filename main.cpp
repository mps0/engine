#include <SDL2/SDL.h>
#include <stdio.h>
#include <chrono>
#include <cmath>

#include "drawLines.hpp"
#include "image.hpp"
#include "vector.hpp"
#include "pipeline.hpp"
#include "matrix.hpp"

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


    bool run = true;
    int i = 0;
    float fps = 0.f;
    while(run) {
        auto exec_start = std::chrono::high_resolution_clock::now();

        i++;
        i = i % 2000;
        float pi = 3.141592;
        float alpha = (i / 1000.f) * pi;

        float cosa = cos(alpha);
        float sina = sin(alpha);


        Mat4f rotY = Mat4f();
        rotY.c0 = Vec4f(cosa, 0.f, -sina, 0.f);
        rotY.c1 = Vec4f(0.f, 1.f, 0.f, 0.f);
        rotY.c2 = Vec4f(sina, 0.f, cosa, 0.f);
        rotY.c3 = Vec4f(0.f, 0.f, 0.f, 1.f);

        Mat4f trans = Mat4f();
        trans.c0 = Vec4f(1.f, 0.f, 0.f, 0.f);
        trans.c1 = Vec4f(0.f, 1.f, 0.f, 0.f);
        trans.c2 = Vec4f(0.f, 0.f, 1.f, 0.f);
        trans.c3 = Vec4f(0.f, 0.f, 2.f, 1.f);

        Mat4f trans2 = Mat4f();
        trans2.c0 = Vec4f(1.f, 0.f, 0.f, 0.f);
        trans2.c1 = Vec4f(0.f, 1.f, 0.f, 0.f);
        trans2.c2 = Vec4f(0.f, 0.f, 1.f, 0.f);
        trans2.c3 = Vec4f(0.f, 0.f, -2.f, 1.f);



        Vec4f p0 = Vec4f(-0.5f, -0.5f, -2.f, 1.f);
        Vec4f p1 = Vec4f(0.5f, -0.5f, -2.f, 1.f);
        Vec4f p2 = Vec4f(0.0f, 0.5f, -2.f, 1.f);


        p0 = trans * p0;
        p1 = trans * p1;
        p2 = trans * p2;


        p0 = rotY * p0;
        p1 = rotY * p1;
        p2 = rotY * p2;


        p0 = trans2 * p0;
        p1 = trans2 * p1;
        p2 = trans2 * p2;



        pipeline(&image, p0, p1, p2);

        SDL_UpdateTexture(texture, NULL, image.pixels, SCREEN_WIDTH * sizeof(Uint32));
        image.clear();

        if(SDL_PollEvent(&event) && event.type == SDL_QUIT){
            break;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        auto exec_end = std::chrono::high_resolution_clock::now();
        auto exec_time = std::chrono::duration_cast<std::chrono::milliseconds>(exec_end - exec_start).count(); 
        //float exec_time = std::chrono::duration_cast<std::chrono::microseconds>(exec_end - exec_start).count(); 
        //fps = 1000000.f / exec_time;
        //printf("FPS: %i\n", (int)fps);
        //
        printf("execution time: %i ms\n", exec_time);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}










