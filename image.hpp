#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <cfloat>
#include <stdio.h>

#include "vector.hpp"



class Image{
    public:
    const unsigned short width, height;
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

        void setPixel(unsigned int x, unsigned int y, Vec3i RGB, float zDepth){

           int mask = 255; //1111 1111
           unsigned int color = 0;

           color |= RGB.x;
           color |= (RGB.y << 8);
           color |= (RGB.z << 16);

            
           unsigned int idx = y * width + x; 


          if (zDepth < zBuffer[idx]) {
           pixels[idx] = color;
           zBuffer[idx] = zDepth;
           } 
        }
        

        void clear() {

        int t  = 100.f;

         memset(pixels, 0, width * height * sizeof(unsigned int));

         //can't use memset for floats
         //memset(zBuffer, t, width * height * sizeof(float));
         
         for(int i = 0; i < (width * height); i++) {
            zBuffer[i] = FLT_MAX;
         }

        }

};















#endif
