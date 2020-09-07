#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "vector.hpp"


class Image{
    public:
    const unsigned short width, height;
    unsigned int* pixels;

        Image() : width(0), height(0) {}
        Image(const unsigned short width, const unsigned short height) :width(width), height(height) {
      
         pixels = new unsigned int[width * height];
         //default white background
         memset(pixels, 255, width * height * sizeof(unsigned int));
        }

        ~Image(){
        delete[] pixels;
        }

        void setPixel(unsigned int x, unsigned int y, Vec3i RGB){

           int mask = 255; //1111 1111
           unsigned int color = 0;

           color |= RGB.x;
           color |= (RGB.y << 8);
           color |= (RGB.z << 16);

            
           unsigned int idx = y * width + x; 
           pixels[idx] = color;
        }

        void clear() {

        
         memset(pixels, 255, width * height * sizeof(unsigned int));

        }

};















#endif
