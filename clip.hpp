#ifndef CLIP_HPP
#define CLIP_HPP

#include <vector>

#include "vector.hpp"
#include "vertex.hpp"
#include "plane.hpp"

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720


const int INSIDE = 0; 
const int LEFT = 1;
const int RIGHT = 2;
const int BOTTOM = 4;
const int TOP = 8;
const int FRONT = 16;
const int BACK = 32;

const int XMINCLIP = 0 << 4;
const int XMAXCLIP = (SCREEN_WIDTH - 1) << 4;
const int YMINCLIP = 0 << 4;
const int YMAXCLIP = (SCREEN_HEIGHT -1) << 4;


int computeCode3D(Vec4f pp) {
    int result = 0;


    //TODO: make this part of camera class
    float n = -1.f;
    float f = -30.f;
    //float alpha = 3.141592 * .5f;
    float alpha = 3.141592 * .8f;
    float t = tan(alpha * .5f) * abs(n);
    float r = SCREEN_WIDTH / SCREEN_HEIGHT * t;
    float b = - t;
    float l = - r;


    //printf("n: %f, alpha: %f, t: %f, r: %f, b: %f, l: %f\n",n,alpha,t,r,b,l);

    float offset = 0.000001f; //deal with floating point error
    if ((pp.x - offset) > pp.w * l) {
        result |= LEFT;
    }
    else if ((pp.x + offset) < pp.w * r) {
        result |= RIGHT;
    }

    if ((pp.y - offset) > pp.w * b) {
        result |= BOTTOM;
    } 
    else if ((pp.y + offset) < pp.w * t) {
        result |= TOP;
    }
    if ((pp.z + offset) < pp.w * n) {
       result |= FRONT;
    } 
    
    else if ((pp.z - offset) > (f * pp.w)) {
       result |= BACK;
    }
    return result;


    //float offset = 0.000001f; //deal with floating point error
    //if ((pp.x - offset) > -pp.w) {
    //    result |= LEFT;
    //}
    //else if ((pp.x + offset) < pp.w) {
    //    result |= RIGHT;
    //}

    //if ((pp.y - offset) > -pp.w) {
    //    result |= BOTTOM;
    //} 
    //else if ((pp.y + offset) < pp.w) {
    //    result |= TOP;
    //}
    //if ((pp.z + offset) < -pp.w) {
    //   result |= FRONT;
    //} 
    //
    //else if ((pp.z - offset) > (-2.f * pp.w)) {
    //   result |= BACK;
    //}
    //return result;
}


bool cohenSutherlandClip3D(Vec4f pp0, Vec4f pp1) {

    int c0 = computeCode3D(pp0);
    int c1 = computeCode3D(pp1);


    //hidden
    if(c0 & c1) {
        return true;
    }

    else {
        return false;
    }
}




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


bool cohenSutherlandClip(Vec2i &p0, Vec2i &p1) {

    p0.x <<= 4;
    p0.y <<= 4;
    p1.x <<= 4;
    p1.y <<= 4;

    int c0 = computeCode(p0);
    int c1 = computeCode(p1);

    bool run = true;

    bool visible = true;


    while(run) {


        //both inside
        if(!(c0 | c1)) {
            run = false;
            break;
        } 
        //both in one of the areas defined above
        else if(c0 & c1) {
                printf("hi\n");
            run = false;
            visible = false;
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

    return visible;
}


#endif

