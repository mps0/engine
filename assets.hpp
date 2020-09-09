#ifndef ASSETS_HPP
#define ASSETS_HPP

#include "vector.hpp"
#include "pipeline.hpp"


class Asset{
    public:
    Attributes*   vBuffer;
    int*    iBuffer;
    int num_triangles;

    Asset() : vBuffer(nullptr), iBuffer(nullptr) {}
};



class Checkerboard : public Asset {
    public:
    Vec3f origin;
    float width, height;
    int rows, cols;


    Checkerboard (Vec3f origin, float width, float height, int rows, int cols) : 
        origin(origin), width(width), height(height), rows(rows), cols(cols) {

     num_triangles = rows * cols * 2;

    vBuffer = new Attributes[(rows + 1) * (cols + 1)];
    iBuffer = new int[num_triangles * 3];



    float xstep = width / cols;
    float ystep = height / rows;

    float xstart = origin.x - 0.5f * width;
    float ystart = origin.y + 0.5f * height;

    for (int i = 0; i < rows + 1; i++) {
        for (int j = 0; j < cols + 1; j++) {

           vBuffer[i * (cols + 1) + j].pos = Vec4f(j * xstep + xstart, ystart - i* ystep, -1.f, 1.f);
            //printf("vBuffer[%i] = (%f, %f, %f, %f)\n", i * (cols + 1) + j, j * xstep + xstart, -i * ystep + ystart, -2.f, 1.f);
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int startIdx = 6 * (i * (cols) + j);
            iBuffer[startIdx] = (i + 1) * (cols + 1) + j;
            iBuffer[startIdx + 1] = i * (cols + 1) + j;
            iBuffer[startIdx + 2] = i * (cols + 1) + j + 1;

            //printf("iBuffer[%i] = %i\n", startIdx, iBuffer[startIdx]);
            //printf("iBuffer[%i] = %i\n", startIdx + 1, iBuffer[startIdx + 1]);
            //printf("iBuffer[%i] = %i\n", startIdx + 2, iBuffer[startIdx + 2]);

            iBuffer[startIdx + 3] = i * (cols + 1) + j + 1;;
            iBuffer[startIdx + 4] = (i + 1) * (cols + 1) + j + 1;
            iBuffer[startIdx + 5] = (i + 1) * (cols + 1) + j;

            //printf("iBuffer[%i] = %i\n", startIdx + 3, iBuffer[startIdx + 3]);
            //printf("iBuffer[%i] = %i\n", startIdx + 4, iBuffer[startIdx + 4]);
            //printf("iBuffer[%i] = %i\n", startIdx + 5, iBuffer[startIdx + 5]);
        }
    }



        }

};




















#endif
