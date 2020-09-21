#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <cfloat>
#include <stdio.h>

#include "vector.hpp"
#include "matrix.hpp"



class Image{
    public:
        unsigned short width, height;
        unsigned int* pixels;
        float* zBuffer;

        Image() : width(0), height(0) {}
        Image(const unsigned short width, const unsigned short height) :width(width), height(height) {

            pixels = new unsigned int[width * height];
            zBuffer = new float[width * height];
            clear();
        }

        ~Image(){
            delete[] pixels;
            delete[] zBuffer;
        }

        virtual void setPixel(unsigned int x, unsigned int y, Vec3i RGB, float zDepth){

            unsigned int color = 0;

            color |= RGB.x;
            color |= (RGB.y << 8);
            color |= (RGB.z << 16);


            unsigned int idx = y * width + x; 


            if (zDepth > zBuffer[idx]) {
                pixels[idx] = color;
                zBuffer[idx] = zDepth;
            } 
        }


        virtual void clear() {

            memset(pixels, 0, width * height * sizeof(unsigned int));

            //can't use memset for floats
            for(int i = 0; i < (width * height); i++) {
                zBuffer[i] = -FLT_MAX;
            }

        }

};


class ShadowMap : public Image{
    public:


        ShadowMap()  {}

        ShadowMap(const unsigned short widthSM, const unsigned short heightSM) {
            width = widthSM;
            height = heightSM;
            zBuffer = new float[width * height];
            clear();
        }

        void setPixel(unsigned int x, unsigned int y, Vec3i RGB, float zDepth){

            unsigned int idx = y * width + x; 
            if (zDepth > zBuffer[idx]) {
                zBuffer[idx] = zDepth;
            } 

        }

        float getMap(unsigned int x, unsigned int y, float zDepth) {

            
            unsigned int idx = y * width + x; 


            float offset = 0.01f;
            if(zDepth >= (zBuffer[idx] - offset)) {

                return  1.f + (zBuffer[idx] + 1.f) / 29.f; //hardcoded for n= -1, f= -30

            }


            return 0.f;
        }


        void clear() {

            for(int i = 0; i < (width * height); i++) {
                zBuffer[i] = -FLT_MAX;
            }

        }


};















#endif
