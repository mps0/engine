
#include <SDL2/SDL.h>
#include <stdio.h>
#include <chrono>
#include <cmath>

#include "drawLines.hpp"
#include "image.hpp"
#include "vector.hpp"
#include "pipeline.hpp"
#include "matrix.hpp"
#include "assets.hpp"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640


int main(void) {
    SDL_Event event;
    SDL_Window* window;
    SDL_Surface* surface;

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("SDL test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    surface = SDL_GetWindowSurface(window);

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture * texture = SDL_CreateTexture(renderer,
            SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    Image image = Image(SCREEN_WIDTH, SCREEN_HEIGHT);

            int tx = 5;

            float ty = tx;

            printf("tx: %i, ty: %f\n", tx, ty);

        Asset* asset2 = new Grid(Vec3f(0.f, 0.f, 0.f), 2.f, 2.f, 25, 25);
        Asset* asset = new Box(Vec3f(0.f, 0.f, 0.f), 0.1f, 0.1f, 0.1f);

    bool run = true;
    int i = 0;
    float fps = 0.f;
    while(run) {
        auto exec_start = std::chrono::high_resolution_clock::now();

//        i++;
//        i = i % 2000;
//        float pi = 3.141592;
//        float alpha = (i / 1000.f) * pi;
//
//        float cosa = cos(alpha);
//        float sina = sin(alpha);
//
//
//        Mat4f rotY = Mat4f();
//        rotY.c0 = Vec4f(cosa, 0.f, -sina, 0.f);
//        rotY.c1 = Vec4f(0.f, 1.f, 0.f, 0.f);
//        rotY.c2 = Vec4f(sina, 0.f, cosa, 0.f);
//        rotY.c3 = Vec4f(0.f, 0.f, 0.f, 1.f);
//
//        Mat4f rotZ = Mat4f();
//        rotZ.c0 = Vec4f(cosa, sina, 0.f, 0.f);
//        rotZ.c1 = Vec4f(-sina, cosa, 0.f,  0.f);
//        rotZ.c2 = Vec4f(0.f, 0.f, 1.f, 0.f);
//        rotZ.c3 = Vec4f(0.f, 0.f, 0.f, 1.f);
//
//        //Vec4f p0 = Vec4f(-2.0f, -1.75f, -2.f, 1.f);
//        //Vec4f p1 = Vec4f(2.0f, -1.75f, -2.f, 1.f);
//        //Vec4f p2 = Vec4f(0.0f, std::sqrt(13.f) - 1.75f, -2.f, 1.f);
//
//        Vec4f p0 = Vec4f(-2.0f, -1.75f, -8.f, 1.f);
//        Vec4f p1 = Vec4f(2.0f, -1.75f, -8.f, 1.f);
//        Vec4f p2 = Vec4f(0.0f, std::sqrt(13.f) - 1.75f, -8.f, 1.f);
//
//
//        float cx = (p0.x + p1.x + p2.x) / 3.f;
//        float cy = (p0.y + p1.y + p2.y) / 3.f;
//        float cz = (p0.z + p1.z + p2.z) / 3.f;
//
//
//        Mat4f trans = Mat4f();
//        trans.c0 = Vec4f(1.f, 0.f, 0.f, 0.f);
//        trans.c1 = Vec4f(0.f, 1.f, 0.f, 0.f);
//        trans.c2 = Vec4f(0.f, 0.f, 1.f, 0.f);
//        trans.c3 = Vec4f(-cx, -cy, -cz, 1.f);
//
//        Mat4f trans2 = Mat4f();
//        trans2.c0 = Vec4f(1.f, 0.f, 0.f, 0.f);
//        trans2.c1 = Vec4f(0.f, 1.f, 0.f, 0.f);
//        trans2.c2 = Vec4f(0.f, 0.f, 1.f, 0.f);
//        trans2.c3 = Vec4f(cx, cy, cz, 1.f);


//        p0 = trans * p0; 
//        p1 = trans * p1;
//        p2 = trans * p2;
//
//        p0 = rotY * p0;
//        p1 = rotY * p1;
//        p2 = rotY * p2;
//
//
//        p0 = trans2 * p0;
//        p1 = trans2 * p1;
//        p2 = trans2 * p2;
//
//        Attributes v0, v1, v2;
//
//        v0.pos = p0;
//        v0.color = Vec4f(1.f, 0.f, 0.f, 1.f);
//        v1.pos = p1;
//        v1.color = Vec4f(0.f, 1.f, 0.f, 1.f);
//        v2.pos = p2;
//        v2.color = Vec4f(0.f, 0.f, 1.f, 1.f);
//


        Attributes v0, v1, v2;
       for(int k = 0; k < asset->num_triangles; k++) {
        
            v0 = asset->Asset::vBuffer[asset->Asset::iBuffer[3 * k]];
            v0.color = Vec4f(0.f, 0.f, 0.f, 1.f);

            v1 = asset->Asset::vBuffer[asset->Asset::iBuffer[3 * k + 1]];
            v1.color = Vec4f(0.f, 0.f, 0.f, 1.f);

            v2 = asset->Asset::vBuffer[asset->Asset::iBuffer[3 * k + 2]];
            v2.color = Vec4f(0.f, 0.f, 0.f, 1.f);
         //printf("p0.x: %f, p0.y: %f, p0.z: %f, p0.w %f\n", v0.pos.x, v0.pos.y, v0.pos.z, v0.pos.w);
         //printf("p1.x: %f, p1.y: %f, p1.z: %f, p1.w %f\n", v1.pos.x, v1.pos.y, v1.pos.z, v1.pos.w);
         //printf("p2.x: %f, p2.y: %f, p2.z: %f, p2.w %f\n", v2.pos.x, v2.pos.y, v2.pos.z, v2.pos.w);


        pipeline(&image, v0, v1, v2, true);

        }
       
       for(int k = 0; k < asset2->num_triangles; k++) {
        
            v0 = asset2->Asset::vBuffer[asset2->Asset::iBuffer[3 * k]];
            v0.color = Vec4f(1.f, 1.f, 1.f, 1.f);

            v1 = asset2->Asset::vBuffer[asset2->Asset::iBuffer[3 * k + 1]];
            v1.color = Vec4f(1.f, 1.f, 1.f, 1.f);

            v2 = asset2->Asset::vBuffer[asset2->Asset::iBuffer[3 * k + 2]];
            v2.color = Vec4f(1.f, 1.f, 1.f, 1.f);
         //printf("p0.x: %f, p0.y: %f, p0.z: %f, p0.w %f\n", v0.pos.x, v0.pos.y, v0.pos.z, v0.pos.w);
         //printf("p1.x: %f, p1.y: %f, p1.z: %f, p1.w %f\n", v1.pos.x, v1.pos.y, v1.pos.z, v1.pos.w);
         //printf("p2.x: %f, p2.y: %f, p2.z: %f, p2.w %f\n", v2.pos.x, v2.pos.y, v2.pos.z, v2.pos.w);


        pipeline(&image, v0, v1, v2, false);

        }



        SDL_UpdateTexture(texture, NULL, image.pixels,  SCREEN_WIDTH * sizeof(Uint32));
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










