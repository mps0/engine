#ifndef DRAWTRIANGLE_HPP
#define DRAWTRIANGLE_HPP

#include "image.hpp"
#include "vector.hpp"

void drawTriangle(Vec2i p0, Vec2i p1, Vec2i p2, Vec4f c0, Vec4f c1, Vec4f c2, Image* image) {


    int x0, y0, x1, y1, x2, y2;

    x0 = p0.x;
    y0 = p0.y;

    x1 = p1.x;
    y1 = p1.y;

    x2 = p2.x;
    y2 = p2.y;


    int xMin = x0;
    int xMax = x0;
    int yMin = y0;
    int yMax = y0;

    if (x1 < xMin) xMin = x1;
    else if (x1 > xMax) xMax = x1;
    if(y1 < yMin) yMin = y1;
    else if (y1 > yMax) yMax = y1;

    if (x2 < xMin) xMin = x2;
    else if (p2.x > xMax) xMax = x2;
    if(y2 < yMin) yMin = y2;
    else if (p2.y > yMax) yMax = y2;


    Vec3i RGB = Vec3i(0, 0, 0);
    int x, y;
    for(y = yMin; y < yMax + 1; y++) {
        for(x = xMin; x < xMax + 1; x++) {

            float f21xy = -y *(x1 - x2) + x * (y1 - y2) + x1 * y2 - x2 * y1;
            float f21x0y0 = -y0 *(x1 - x2) + x0 * (y1 - y2) + x1 * y2 - x2 * y1;

            float alpha = f21xy / f21x0y0;



            float f20xy = -y *(x0 - x2) + x * (y0 - y2) + x0 * y2 - x2 * y0;
            float f20x1y1 = -y1 *(x0 - x2) + x1 * (y0 - y2) + x0 * y2 - x2 * y0;

            float beta = f20xy / f20x1y1;

            float f01xy = -y *(x1 - x0) + x * (y1 - y0) + x1 * y0 - x0 * y1;
            float f01x2y2 = -y2 *(x1 - x0) + x2 * (y1 - y0) + x1 * y0 - x0 * y1;

            float gamma = f01xy / f01x2y2;


            if (alpha > 0.f) {
                if (beta > 0.f) {
                    if (gamma > 0.f) {

                        int R = (int)(alpha * 255.f); 
                        int G = (int)(beta * 255.f); 
                        int B = (int)(gamma * 255.f); 

                        RGB.x = R;
                        RGB.y = G;
                        RGB.z = B;

                        image->setPixel(x, y, RGB);

                    }
                }
            }
        } 
    }
}





































#endif
