#ifndef DRAWLINES_HPP 
#define DRAWLINES_HPP

#include <algorithm>
#include <cmath>
#include <stdio.h>

#include "vector.hpp"
#include "image.hpp"


void drawLines(Vec2i p0, Vec2i p1, Image* image) {

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

    if(abs(dx) > abs(dy) && dy >= 0) {
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
    } else if(abs(dx) > abs(dy) && dy < 0) {
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
