#ifndef DRAWLINES_HPP 
#define DRAWLINES_HPP

#include "vector.hpp"
#include "image.hpp"


void drawLines(Image* image) {

    vec2i p0 = vec2i(0, 0);
    vec2i p1 = vec2i(image->width - 1, image->height - 1);

    int X0 = p0.x;
    int Y0 = p0.y;

    int X1 = p1.x;
    int Y1 = p1.y;

    int dx = X1 - X0;
    int dy = Y1 - Y0;

    int d = dy -  dx / 2;

    int x = X0;
    int y = Y0;

    image->setPixel(x, y, 0);

    while(x < X1) {
        x++;

        if (d < 0) {

            d = d + dy;           

        } else {

            d = d + dy - dx;
            y++;

        }
    image->setPixel(x, y, 0);
    }
}


#endif
