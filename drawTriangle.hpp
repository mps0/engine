#ifndef DRAWTRIANGLE_HPP
#define DRAWTRIANGLE_HPP

#include "image.hpp"
#include "vector.hpp"
#include "shader.hpp"




/* --x+---->                                       
 * |                                       2, gamma
 * y                                      / \
 * +                                     /   \
 * ↓                                    /     \
 *                                     /       \
 *                            alpha, 0/_________\1, beta
 *
 * The general form of a line is f(x,y) = Ax + By + C = 0
 * The value of the function at a (x,y) pair that is not on the line is non-zero.
 * The magnitude of the value is proportional to the perpinduclar distance from the 
 * (x,y) pair to the line. We can use this to compute the barycentric coordinates of
 * a triangle by noting the barycentric value of one of the 3 triangle vertices, is 
 * just the scaled perpinduclar distance from the vertex to the line segment joining the
 * other two vertices.
 *
 *
 * Consider the line, which segment 02, lies on:
 *
 * F12(x,y) = 0  (To the left of the line  F12 > 0, and to the right F12 < 0;)
 *
 * So the perpindicualr distance to the line is:
 *
 * d = k * F12(x, y)   (k is the scaling factor)
 *
 * At (x,y) = (x0, y0) we know that alpha is 1. We can use this to determine k:
 * 
 * 1 = k * F12(x0, y0)
 * => k = 1 / F12(x0, y0)
 * 
 * Plugging it back in,
 * 
 * alpha = F12(x, y) / F12(x0, y0);
 * 
 * And following the same logic,
 * 
 * beta = F20(x, y) / F20(x1, y1);
 * gamma = F01(x, y) / F01(x2, y2);
 * 
 * Now we have to find each line function. First, let's find C by expressing F21 in slope intercept form:
 * 
 * y = mx + b
 * 
 * Plugging in triangle vertex information:
 * 
 * y = (y2 - y1) / (x2 - x1)x + b
 * (x2 - x1)y = (y2 - y1)x  + C
 * (y2 - y1)x + (x1 - x2)y + C = 0 (This is the general form again, Ax + By + C = 0)
 * C = (x2 - x1)y + (y1 - y2)x = 
 * C = x2 * y - x1 * y + y1 * x - y2 * x
 * 
 * We know that (x2, y2) lies on the line:
 * C = x2 * y2 - x1 * y2 + y1 * x2 - y2 * x2
 * C = x2 * y1 - y2 * x1
 *
 * So F12(x,y) = (y2 - y1)x  + (x1 - x2)y + x2 * y1 - y2 * x1
 *
 * The other lines follow analogously.
 *
 * The algorithm can be made incremental by noting that in both the x and y loops over the image we increase by one, and:
 * 
 * F(x + 1, y) = A(x + 1) + By + C = Ax + A + By + C = F(x,y) + A
 * F(x, y + 1) = Ax + B(y + 1) + C = Ax + By + B + C = F(x,y) + B
 */

//void drawTriangle(Vec2i p0, Vec2i p1, Vec2i p2, Vec4f c0, Vec4f c1, Vec4f c2, Image* image) {
//
int clamp(int x, int min, int max) {

    if (x < min) x = min;
    else if (x > max) x = max;

}



//void drawTriangle(Vec2i p0, Vec2i p1, Vec2i p2, Vec4f c0, Vec4f c1, Vec4f c2, Vec3f n0, Vec3f n1, Vec3f n2, float z0, float z1, float z2, Image* image) {
void drawTriangle(Vertex v0, Vertex v1, Vertex v2, Image* image) {

    int x0, y0, x1, y1, x2, y2;

    x0 = (int)v0.pos.x;
    y0 = (int)v0.pos.y;

    x1 = (int)v1.pos.x;
    y1 = (int)v1.pos.y;

    x2 = (int)v2.pos.x;
    y2 = (int)v2.pos.y;





    int xMin = x0;
    int xMax = x0;
    int yMin = y0;
    int yMax = y0;



    if (x1 < xMin) xMin = x1;
    else if (x1 > xMax) xMax = x1;
    if(y1 < yMin) yMin = y1;
    else if (y1 > yMax) yMax = y1;
    if (x2 < xMin) xMin = x2;
    else if (x2 > xMax) xMax = x2;
    if(y2 < yMin) yMin = y2;
    else if (y2 > yMax) yMax = y2;

    //printf("xMin: %i, xMax: %i\n", xMin, xMax);
    //printf("yMin: %i, yMax: %i\n", yMin, yMax);

    xMin = clamp(xMin, 0, SCREEN_WIDTH - 1);
    xMax = clamp(xMax, 0, SCREEN_WIDTH - 1);

    yMin = clamp(yMin, 0, SCREEN_HEIGHT - 1);
    yMax = clamp(yMax, 0, SCREEN_HEIGHT - 1);

    //printf("xMin: %i, xMax: %i\n", xMin, xMax);
    //printf("yMin: %i, yMax: %i\n", yMin, yMax);


    int A01 = y1 - y0;
    int B01 = x0 - x1; 
    int C01 = x1 * y0 - y1 * x0;
    int f01xy = A01 * xMin  + B01 * yMin + C01;
    float f01x2y2 = A01 * x2  + B01 * y2 + C01;


    int A12 = y2 - y1;
    int B12 = x1 - x2; 
    int C12 = x2 * y1 - y2 * x1;
    int f12xy = A12 * xMin  + B12 * yMin + C12;
    float f12x0y0 = A12 * x0  + B12 * y0 + C12;


    int A20 = y0 - y2;
    int B20 = x2 - x0; 
    int C20 = x0 * y2 - y0 * x2;
    int f20xy = A20 * xMin  + B20 * yMin + C20;
    float f20x1y1 = A20 * x1  + B20 * y1 + C20;


    //printf("f12xy: %i, f20xy: %i, f01xy: %i", f12xy, f20xy, f01xy);

    int x, y;
    for(y = yMin; y < yMax + 1; y++) {

        int f12xyXLOOP = f12xy;
        int f20xyXLOOP = f20xy;
        int f01xyXLOOP = f01xy;
        for(x = xMin; x < xMax + 1; x++) {
                //printf("f21xyXLOOP: %f, f20xyXLOOP: %f, f01xyXLOOP: %f\n", f21xyXLOOP, f20xyXLOOP, f01xyXLOOP);

            //TODO: TRIANGLE PIXEL EDGES
            if((f12xyXLOOP | f20xyXLOOP | f01xyXLOOP) >= 0) { 


                float alpha = f12xyXLOOP / f12x0y0;
                float beta = f20xyXLOOP / f20x1y1;
                float gamma = f01xyXLOOP / f01x2y2;


                fragmentShader(v0, v1, v2, Vec3f(alpha, beta, gamma), x, y, image);

            }
            f12xyXLOOP = f12xyXLOOP + A12;
            f20xyXLOOP = f20xyXLOOP + A20;
            f01xyXLOOP = f01xyXLOOP + A01;
        } 
        f12xy = f12xy + B12;
        f20xy = f20xy + B20;
        f01xy = f01xy + B01;
    }
}

#endif
