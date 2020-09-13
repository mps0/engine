
#include <SDL2/SDL.h>
#include <stdio.h>
#include <chrono>
#include <cmath>
#include <vector>

#include "drawLine.hpp"
#include "image.hpp"
#include "vector.hpp"
#include "pipeline.hpp"
#include "matrix.hpp"
#include "assets.hpp"
#include "camera.hpp"
#include "vertex.hpp"



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



    //Asset* triangle = new Triangle(Vec3f(0.f, -0.5f, -1.f), Vec3f(0.f, 0.5f, -1.f), Vec3f(-2.5f, 0.5f, -1.f),Vec4f(1.f, 0.f, 0.f, 1.f), Vec4f(0.f, 1.f, 0.f, 1.f), Vec4f(0.f, 0.f, 1.f, 1.f));
    Asset* triangle = new Triangle(Vec3f(-1.0f, -0.5f, -1.f), Vec3f(0.f, -0.5f, -1.f), Vec3f(-1.0f, 0.5f, -1.f),Vec4f(1.f, 0.f, 0.f, 1.f), Vec4f(0.f, 1.f, 0.f, 1.f), Vec4f(0.f, 0.f, 1.f, 1.f));
    //Asset* grid = new Grid(Vec3f(0.f, 0.f, -1.f), 2.f, 2.f, 2, 2); //Asset* box = new Box(Vec3f(0.f, 0.f, -1.f), 0.5f, 0.5f, 0.5f);

    std::vector<Asset*> assets;
    assets.push_back(triangle);
    //assets.push_back(grid);
    //assets.push_back(box);

    Camera* cam = new Camera(Vec3f(0.f, 1.f, 0.f), Vec3f(0.f, 0.f, -1.f), Vec3f(0.f, 0.f, 5.f));

    bool run = true; 
    while(run) {
        auto exec_start = std::chrono::high_resolution_clock::now();

        while( SDL_PollEvent( &event ) != 0 ){
            switch(event.type) {

                case SDL_QUIT: {
                    run = false;
                    break;
                               }
                case SDL_MOUSEMOTION:{
                    unsigned int button = SDL_GetMouseState(NULL, NULL);
                    if (button & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                        cam->rotate(event.motion.xrel, -event.motion.yrel);
                    }
                    else if (button & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
                        cam->roll(event.motion.xrel);
                    }
                    break;
                                     }
                 case SDL_KEYDOWN: {
                        switch( event.key.keysym.sym )
                        {
                            case SDLK_w:
                                cam->pos = cam->pos + cam->sensitivity * cam->forward; 
                                break;

                            case SDLK_s:
                                cam->pos = cam->pos - cam->sensitivity * cam->forward; 
                                break;

                            case SDLK_a:
                                cam->pos = cam->pos - cam->sensitivity * cam->u;
                                break;

                            case SDLK_d:
                                cam->pos = cam->pos + cam->sensitivity * cam->u;
                                break;

                        }

                    }
            }
        }
                                     

                                     Vertex v0, v1, v2;
                                     for(Asset* asset : assets) {
                                         for(int  k = 0; k < asset->num_triangles; k++) {

                                             v0 = asset->Asset::vBuffer[asset->Asset::iBuffer[3 * k]];
                                             v1 = asset->Asset::vBuffer[asset->Asset::iBuffer[3 * k + 1]];
                                             v2 = asset->Asset::vBuffer[asset->Asset::iBuffer[3 * k + 2]];

                                             pipeline(&image, v0, v1, v2, cam);
                                         }
                                     }


                                     SDL_UpdateTexture(texture, NULL, image.pixels,  SCREEN_WIDTH * sizeof(Uint32));
                                     image.clear();


                                     SDL_RenderClear(renderer);
                                     SDL_RenderCopy(renderer, texture, NULL, NULL);
                                     SDL_RenderPresent(renderer);
                                     auto exec_end = std::chrono::high_resolution_clock::now();
                                     auto exec_time = std::chrono::duration_cast<std::chrono::milliseconds>(exec_end - exec_start).count(); 
                                     //float exec_time = std::chrono::duration_cast<std::chrono::microseconds>(exec_end - exec_start).count(); 
                                     //float fps = 1000000.f / exec_time;
                                     //printf("FPS: %i\n", (int)fps);
                                     //
                                     //printf("execution time: %i ms\n", exec_time);
            }


            SDL_DestroyTexture(texture);
            SDL_DestroyRenderer(renderer);

            SDL_DestroyWindow(window);
            SDL_Quit();
            return EXIT_SUCCESS;
        }










