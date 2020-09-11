#ifndef DRAWLINES_HPP 
#define DRAWLINES_HPP

#include <algorithm>
#include <cmath>
#include <stdio.h>

#include "vector.hpp"
#include "image.hpp"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640

const int INSIDE = 0; 
const int LEFT = 1;
const int RIGHT = 2;
const int BOTTOM = 4;
const int TOP = 8;

const int XMINCLIP = 0 << 4;
const int XMAXCLIP = SCREEN_WIDTH << 4;
const int YMINCLIP = 0 << 4;
const int YMAXCLIP = SCREEN_HEIGHT << 4;

int computeCode(Vec2i p) {

    int result = 0;

    if (p.x < XMINCLIP) {

        result |= LEFT;

    } else if (p.x > XMAXCLIP) {

        result |= RIGHT;
    }

    if (p.y > YMAXCLIP) { // Y positive down!

        result |= BOTTOM;

    } else if (p.y < YMINCLIP) {

        result |= TOP;
    }

    return result;
}


void cohenSutherlandClip(Vec2i &p0, Vec2i &p1) {

    p0.x <<= 4;
    p0.y <<= 4;
    p1.x <<= 4;
    p1.y <<= 4;

    int c0 = computeCode(p0);
    int c1 = computeCode(p1);

    bool run = true;


    while(run) {


        //both inside
        if(!(c0 | c1)) {
            run = false;
            break;
        } 
        //both in one of the areas defined above
        else if(c0 & c1) {
            run = false;
            break;


        }

        else {


            //at least one point is not inside. So pick largest bitcode to get it.
            int c = c1 > c0 ? c1 : c0;
            int xS, yS, x, y;

            if (c & LEFT) {

                x = XMINCLIP;
                yS = p0.y * (p1.x - p0.x) + (p1.y - p0.y) * (XMINCLIP - p0.x);
                y = yS / (p1.x - p0.x);
            }
            else if (c & RIGHT) {

                x = XMAXCLIP;
                yS = p0.y * (p1.x - p0.x) + (p1.y - p0.y) * (XMAXCLIP - p0.x);
                y = yS / (p1.x - p0.x);
            }
            else if (c & BOTTOM) {
                xS = p0.x * (p1.y - p0.y) + (p1.x - p0.x) * (YMAXCLIP - p0.y);
                x = xS / (p1.y - p0.y);
                y = YMAXCLIP;
            }
            else if (c & TOP) {
                xS = p0.x * (p1.y - p0.y) + (p1.x - p0.x) * (YMINCLIP - p0.y);
                x = xS / (p1.y - p0.y);
                y = YMINCLIP;
            }

            if (c == c0) {
                p0.x = x;
                p0.y = y;

                c0 = computeCode(p0);
            } else {
                p1.x = x;
                p1.y = y;

                c1 = computeCode(p1);
            }
        }
    }
    p0.x >>= 4;
    p0.y >>= 4;
    p1.x >>= 4;
    p1.y >>= 4;
}

/* We can express a line in its general form: F(x,y) = Ax + By + C = 0
 * Starting from slope intercept form, y = mx + b
 *
 * y = dy/dx * x + b
 * F(x, y) = 0 = dy * x - dx * y + dx * b
 *
 *
 * Let's assume that 0 < dy/dx <= 1 (i.e. slope is at most 1) and that the x-axis
 * is the driving axis. This is the midpoint drawing algorithm and what we are
 * essentially doing is chosing the next pixel in the line as the one to the right,
 * (x + 1, y) or the one to the right and up (x + 1, y + 1). To do this, we 
 * evaluate the line function as we draw it and keep track of the accumulated error
 * through a decision variable. The function is evaluated at F(x + 1, y + 0.5) 
 * noting that when a point is above the line F(x + 1, y + 0.5) will be positive,
 * when on it 0, and when below it negative.
 *
 * F(x + 1, y + 0.5) 
 * = dy * (x + 1) - dx (y + 0.5) + dx * b
 * = dy * x + dy - dx * y - dx * 0.5 + dx * b
 * = (dy * dx - dx * y + dx * b) + dy - dx * 0.5
 * = F(x, y) + dy - dx * 0.5
 *
 * We use this midpoint evaluation as our decision variable, D.
 * To get the intial D, we know F(x0, y0) lies on the line and therefore is 0,
 *
 * D0 = F(x0 + 1, y0 + 0.5)
 *    = F(x0, y0) + dy - 0.5 * dx
 *    = dy - 0.5 * dx
 * 
 * To update D for the next iteration if D0 <= 0 (i.e. only X increased):
 *
 * D1 = D(x0 + 2, y0 + 0.5) - D(x0 + 1, y0 + 0.5)
 *    = [F(x0, y0) + 2 * dy - 0.5 * dx] - [F(x0, y0) + dy - 0.5 * x]
 *    = dy 
 *
 * To update D for the next iteration if D0 > 0 (i.e. X and Y increased):
 *
 * D1 = D(x0 + 2, y0 + 1.5) - D(x0 + 1, y0 + 0.5)
 *    = [F(x0, y0) + 2 * dy - 1.5 * dx] - [F(x0, y0) + dy - 0.5 * x]
 *    = dy - dx
 *
 *
 * Since we only care about the signs of the D's, we can multiply them all by 2 to get 
 * rid of the 0.5 factor in D0, and do everything in integer math:
 *
 * D0 = 2 * dy - dx
 *
 * D1 = 2 * dy (if only x increases)
 * D1 = 2 * dy - 2 * dx (if x and y increases)
 *
 * And so on for each Dn.
 *
 */

void drawLine(Vec2i p0, Vec2i p1, Vec4f c0, Vec4f c1, float z0, float z1, Image* image) {

    cohenSutherlandClip(p0, p1);


    bool switchOrder = p0.x > p1.x;

    if (switchOrder) {

        std::swap(p0, p1);
        std::swap(c0, c1);
    }

    int X0 = p0.x;
    int Y0 = p0.y;

    int X1 = p1.x;
    int Y1 = p1.y;

    int dx = X1 - X0;
    int dy = Y1 - Y0;


    int x = X0;
    int y = Y0;

   Vec3i RGB = Vec3i(0, 0, 0);
    if(abs(dx) >= abs(dy) && dy >= 0) {
        int d = 2 * dy - dx ;
        for(x = X0; x < X1 + 1; x++) {

            float t = (float)(x - X0) / (float)(X1 - X0); 
            Vec4f RGBAf = (1.f - t) * c0 + t * c1;
            float zDepth = (1.f - t) * z0 + t * z1;

            RGB.x = (int)(RGBAf.x * 255.f);
            RGB.y = (int)(RGBAf.y * 255.f);
            RGB.z = (int)(RGBAf.z * 255.f);
            
            image->setPixel(x, y, RGB, zDepth);

            if (d > 0) {

                d = d - 2 * dx;           
                y++;
            }

            d = d  + 2 * dy;
        } 
    } else if(abs(dx) >= abs(dy) && dy < 0) {
        int d = 2 * dy + dx ;
        for(x = X0; x < X1 + 1; x++) {

            float t = (float)(x - X0) / (float)(X1 - X0); 
            Vec4f RGBAf = (1.f - t) * c0 + t * c1;
            float zDepth = (1.f - t) * z0 + t * z1;

            RGB.x = (int)(RGBAf.x * 255.f);
            RGB.y = (int)(RGBAf.y * 255.f);
            RGB.z = (int)(RGBAf.z * 255.f);
            image->setPixel(x, y, RGB, zDepth);

            if (d < 0) {

                d = d + 2 * dx;
                y--;
            }

            d = d + 2 * dy;
        }
    } else if (abs(dy) > abs(dx) && dy < 0) {
        int d = 2 * dx + dy ;
        for(y = Y0; y > Y1 - 1; y--) {
            float t = (float)(Y0 - y) / (float)(Y0 - Y1); 
            float zDepth = (1.f - t) * z0 + t * z1;

            Vec4f RGBAf = (1.f - t) * c0 + t * c1;
            RGB.x = (int)(RGBAf.x * 255.f);
            RGB.y = (int)(RGBAf.y * 255.f);
            RGB.z = (int)(RGBAf.z * 255.f);
            image->setPixel(x, y, RGB, zDepth);

            if (d > 0) {

                d = d + 2 * dy;           
                x++;
            }

            d = d  + 2 * dx;
        }    

    } else if (abs(dy) > abs(dx) && dy >= 0) {
        int d = 2 * dx - dy ;

        for(y = Y0; y < Y1 + 1; y++) {

            float t = (float)(y - Y0) / (float)(Y1 - Y0); 
            Vec4f RGBAf = (1.f - t) * c0 + t * c1;
            float zDepth = (1.f - t) * z0 + t * z1;
            RGB.x = (int)(RGBAf.x * 255.f);
            RGB.y = (int)(RGBAf.y * 255.f);
            RGB.z = (int)(RGBAf.z * 255.f);

            image->setPixel(x, y, RGB, zDepth);

            if (d > 0) {

                d = d - 2 * dy;           
                x++;
            }

            d = d  + 2 * dx;
        }
    }

}

#endif
