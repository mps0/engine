#ifndef DRAWTRIANGLE_HPP
#define DRAWTRIANGLE_HPP

#include <algorithm>
#include <cmath>

#include "image.hpp"
#include "vector.hpp"




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
 * F21(x,y) = 0  (To the left of the line (remember, y positive down.) F21 < 0, and to the right F21 > 0;)
 *
 * So the perpindicualr distance to the line is:
 *
 * d = k * F21(x, y)   (k is the scaling factor)
 *
 * At (x,y) = (x0, y0) we know that alpha is 1. We can use this to determine k:
 * 
 * 1 = k * F21(x0, y0)
 * => k = 1 / F21(x0, y0)
 * 
 * Plugging it back in,
 * 
 * alpha = F21(x, y) / F21(x0, y0);
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
 * y = (y1 - y2) / (x1 - x2)x + b
 * (x1 - x2)y = (y1 - y2)x  + C
 * (y1 - y2)x + (x2 - x1)y + C = 0 (This is the general form again, Ax + By + C = 0)
 * C = (x1 - x2)y + (y2 - y1)x = 
 * C = x1 * y - x2 * y + y2 * x - y1 * x
 * 
 * We know that (x2, y2) lies on the line:
 * C = x1 * y2 - x2 * y2 + y2 * x2 - y1 * x2
 * C = x1 * y2 - y1 * x2
 *
 * So F21(x,y) = (y1 - y2)x  + (x2 - x1)y + x1 * y2 - y1 * x2
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



void drawTriangle(Vec2i p0, Vec2i p1, Vec2i p2, Vec4f c0, Vec4f c1, Vec4f c2, float z0, float z1, float z2, Image* image) {


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

    printf("xMin: %i, xMax: %i\n", xMin, xMax);
    printf("yMin: %i, yMax: %i\n", yMin, yMax);

    xMin = clamp(xMin, 0, SCREEN_WIDTH - 1);
    xMax = clamp(xMax, 0, SCREEN_HEIGHT - 1);

    yMin = clamp(yMin, 0, SCREEN_WIDTH - 1);
    yMax = clamp(yMax, 0, SCREEN_HEIGHT - 1);

    printf("xMin: %i, xMax: %i\n", xMin, xMax);
    printf("yMin: %i, yMax: %i\n", yMin, yMax);




    int A21 = y1 - y2;
    int B21 = x2 - x1; 
    int C21 = x1 * y2 - y1 * x2;
    float f21xy = A21 * xMin  + B21 * yMin + C21;
    float f21x0y0 = A21 * x0  + B21 * y0 + C21;

    int A20 = y0 - y2;
    int B20 = x2 - x0; 
    int C20 = x0 * y2 - y0 * x2;
    float f20xy = A20 * xMin  + B20 * yMin + C20;
    float f20x1y1 = A20 * x1  + B20 * y1 + C20;

    int A01 = y1 - y0;
    int B01 = x0 - x1; 
    int C01 = x1 * y0 - y1 * x0;
    float f01xy = A01 * xMin  + B01 * yMin + C01;
    float f01x2y2 = A01 * x2  + B01 * y2 + C01;


    int x, y;
    for(y = yMin; y < yMax + 1; y++) {
        for(x = xMin; x < xMax + 1; x++) {

            float alpha = f21xy / f21x0y0;
            float beta = f20xy / f20x1y1;
            float gamma = f01xy / f01x2y2;


            //TODO: TRIANGLE PIXEL EDGES
            if (alpha >= 0.f) {
                if (beta >= 0.f) {
                    if (gamma >= 0.f) {

                        
                        Vec4f RGBAf = alpha * c0 + beta * c1 + gamma * c2;
                        int R = RGBAf.x * 255.f; 
                        int G = RGBAf.y * 255.f; 
                        int B = RGBAf.z * 255.f; 

                        
                        float depth = alpha * z0 + beta * z1 + gamma * z2;
                        image->setPixel(x, y, Vec3i(R, G, B),depth);

                    }
                }
            }
            if(x == xMax) { //reset x contributions as we move to next row of pixels.
                f21xy = f21xy - (xMax - xMin) * A21;
                f20xy = f20xy - (xMax - xMin) * A20;
                f01xy = f01xy - (xMax - xMin) * A01;
            }
            else {
                f21xy = f21xy + A21;
                f20xy = f20xy + A20;
                f01xy = f01xy + A01;
            }
        } 
        f21xy = f21xy + B21;
        f20xy = f20xy + B20;
        f01xy = f01xy + B01;
    }
}

//void drawQuad(Vec2i p0, Vec2i p1, Vec2i p2, Vec2i p3, Vec4f c0, Vec4f c1, Vec4f c2, Vec4f c3, float z0, float z1, float z2, float z3, Image* image) {
void drawQuad(Vec2i* p, Vec4f* color, float* z, Image* image) {

    Vec2f a, b, c, d;
    
    a = Vec2f(p[0].x, p[0].y);
    b = Vec2f(p[1].x, p[1].y);
    c = Vec2f(p[2].x, p[2].y);
    d = Vec2f(p[3].x, p[3].y);


    Vec2f cent = 0.25f * (a + b + c + d);

    a = a - cent;
    b = b - cent;
    c = c - cent;
    d = d - cent;


   float angles[4];
   int idx[4];

    idx[0] = 0;
    idx[1] = 1;
    idx[2] = 2;
    idx[3] = 3;

    angles[0] = atan2(a.y, a.x);
    angles[1] = atan2(b.y, b.x);
    angles[2] = atan2(c.y, c.x);
    angles[3] = atan2(d.y, d.x);


    bool run = true;
    while(run) {

    if(angles[idx[1]] < angles[idx[0]]) {
        std::swap(idx[1], idx[0]);
        continue;
    }
    if(angles[idx[2]] < angles[idx[1]]) {
        std::swap(idx[2], idx[1]);
        continue;
    }
    if(angles[idx[3]] < angles[idx[2]]) {
        std::swap(idx[3], idx[2]);
        continue;
    }

    run = false;
    }



    //for (int i = 0; i < 4; i++){
    //printf("angles[%i]: %f\n", i, angles[i]);
    //}
    //for (int i = 0; i < 4; i++){
    //printf("idx[%i]: %i\n", i, idx[i]);
    //}


   drawTriangle(p[idx[0]], p[idx[1]], p[idx[2]], color[idx[0]], color[idx[1]], color[idx[2]], z[idx[0]], z[idx[1]], z[idx[2]], image);

   drawTriangle(p[idx[2]], p[idx[3]], p[idx[0]], color[idx[2]], color[idx[3]], color[idx[0]], z[idx[2]], z[idx[3]], z[idx[0]], image);


   //drawTriangle(p[0], p[2], p[3], c[0], c[2], c[3], z[0], z[2], z[3], image);

   //drawTriangle(p[0], p[3], p[1], c[0], c[3], c[1], z[0], z[3], z[1], image);
   //drawTriangle(p[1], p[3], p[0], c[1], c[3], c[0], z[1], z[3], z[0], image);









}

















#endif
