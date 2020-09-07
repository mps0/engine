#ifndef DRAWTRIANGLE_HPP
#define DRAWTRIANGLE_HPP



#include "drawLines.hpp"
#include "image.hpp"
#include "vector.hpp"

void drawTriangle(Vec2i p0, Vec2i p1, Vec2i p2, Vec4f c0, Vec4f c1, Vec4f c2, Image* image) {

    //scanline algorithm with beta = f(x,y) / f(xb, yb) 
    //

    float x0, y0, x1, y1, x2, y2;
    
    x0 = p0.x;
    y0 = p0.y;

    x1 = p1.x;
    y1 = p1.y;

    x2 = p2.x;
    y2 = p2.y;

    float xMin = x0;
    float xMax = x0;
    float yMin = y0;
    float yMax = y0;

    if (x1 < xMin) xMin = x1;
    else if (x1 > xMax) xMax = x1;
    if(y1 < yMin) yMin = y1;
    else if (y1 > yMax) yMax = y1;

    if (x2 < xMin) xMin = x2;
    else if (p2.x > xMax) xMax = x2;
    if(y2 < yMin) yMin = y2;
    else if (p2.y > yMax) yMax = y2;


    Vec3f p0f = Vec3f(x0, y0, 0.f);
    Vec3f p1f = Vec3f(x1, y1, 0.f);
    Vec3f p2f = Vec3f(x2, y2, 0.f);

    Vec3f L01 = p1f - p0f;
    Vec3f L02 = p2f - p0f;
    Vec3f L12 = p2f - p1f;


    float area = 0.5f * Vec3cross(L01, L02).length();

    unsigned int x, y;

    printf("xMin: %f, xMax: %f, yMin: %f, yMax: %f\n", xMin, xMax, yMin, yMax);


   Vec3i RGB = Vec3i(0, 0, 0);
    for(y = yMin; y < yMax + 1; y++) {
        for(x = xMin; x < xMax + 1; x++) {

            Vec3f L0xy = Vec3f(x, y, 0.f) - p0f;
            Vec3f L1xy = Vec3f(x, y, 0.f) - p1f;

           

           float alpha = (0.5f * Vec3cross(L12, L1xy).length()) / area;
            
           float beta = (0.5f * Vec3cross(L0xy, L02).length()) / area;
           float gamma = (0.5f * Vec3cross(L01, L0xy).length()) / area;

            
           


    //printf("alpha: %f, beta: %f, gamma: %f, x: %f, y: %f\n", alpha, beta, gamma, x, y);

            if ((alpha > 0.f) && (beta > 0.f) && (gamma > 0.f)) {

    printf("alpha: %f, beta: %f, gamma: %f, x: %f, y: %f\n", alpha, beta, gamma, x, y);
            
            image->setPixel(x, y, RGB);

            }

            
             

        }
    }

    






}





























#endif
