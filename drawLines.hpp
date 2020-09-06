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

void drawLines(Vec2i p0, Vec2i p1, Image* image) {

    cohenSutherlandClip(p0, p1);


    bool switchOrder = p0.x > p1.x;

    if (switchOrder) {

        std::swap(p0, p1);
    }

    int X0 = p0.x;
    int Y0 = p0.y;

    int X1 = p1.x;
    int Y1 = p1.y;

    int dx = X1 - X0;
    int dy = Y1 - Y0;


    int x = X0;
    int y = Y0;

    if(abs(dx) >= abs(dy) && dy >= 0) {
        int d = 2 * dy - dx ;
        for(x = X0; x < X1 + 1; x++) {
            //printf("\nX0: %i  Y0:%i  X1: %i  Y1:%i  x: %i y: %i d: %i\n", X0, Y0, X1, Y1, x, y, d);
            image->setPixel(x, y, 0);

            if (d > 0) {

                d = d - 2 * dx;           
                y++;
            }

            d = d  + 2 * dy;
        } 
    } else if(abs(dx) >= abs(dy) && dy < 0) {
        int d = 2 * dy + dx ;
        for(x = X0; x < X1 + 1; x++) {
            //printf("\nX0: %i  Y0:%i  X1: %i  Y1:%i  x: %i y: %i d: %i\n", X0, Y0, X1, Y1, x, y, d);
            image->setPixel(x, y, 0);

            if (d < 0) {

                d = d + 2 * dx;
                y--;
            }

            d = d + 2 * dy;
        }
    } else if (abs(dy) > abs(dx) && dy < 0) {
        int d = 2 * dx + dy ;
        for(y = Y0; y > Y1 - 1; y--) {
            image->setPixel(x, y, 0);
            //printf("\nX0: %i  Y0:%i  X1: %i  Y1:%i  x: %i y: %i d: %i\n", X0, Y0, X1, Y1, x, y, d);

            if (d > 0) {

                d = d + 2 * dy;           
                x++;
            }

            d = d  + 2 * dx;
        }    

    } else if (abs(dy) > abs(dx) && dy >= 0) {
        int d = 2 * dx - dy ;

        for(y = Y0; y < Y1 + 1; y++) {
            image->setPixel(x, y, 0);
            //printf("\nX0: %i  Y0:%i  X1: %i  Y1:%i  x: %i y: %i d: %i\n", X0, Y0, X1, Y1, x, y, d);

            if (d > 0) {

                d = d - 2 * dy;           
                x++;
            }

            d = d  + 2 * dx;
        }
    }

}

#endif
