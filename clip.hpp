#ifndef CLIP_HPP
#define CLIP_HPP

#include <vector>

#include "vector.hpp"
#include "vertex.hpp"
#include "plane.hpp"


const int INSIDE = 0; 
const int LEFT = 1;
const int RIGHT = 2;
const int BOTTOM = 4;
const int TOP = 8;
const int FRONT = 16;
const int BACK = 32;

const int XMINCLIP = 0 << 4;
const int XMAXCLIP = SCREEN_WIDTH << 4;
const int YMINCLIP = 0 << 4;
const int YMAXCLIP = SCREEN_HEIGHT << 4;


int computeCode3D(Vec4f pp) {
    int result = 0;


    float offset = 0.000001f; //deal with floating point error
    if ((pp.x - offset) > -pp.w) {
        result |= LEFT;
    }
    else if ((pp.x + offset) < pp.w) {
        result |= RIGHT;
    }

    if ((pp.y - offset) > -pp.w) {
        result |= BOTTOM;
    } 
    else if ((pp.y + offset) < pp.w) {
        result |= TOP;
    }
    if ((pp.z + offset) < -pp.w) {
       result |= FRONT;
    } 
    
    else if ((pp.z - offset) > (-2.f * pp.w)) {
       result |= BACK;
    }
    return result;
}


std::vector<Vertex> cohenSutherlandClip3D(Vec4f p0, Vec4f p1, Vertex vp0, Vertex vp1, Plane L, Plane R, Plane BO, Plane T, Plane F, Plane BA, bool &change0, bool &change1) {

    int c0 = computeCode3D(vp0.pos);
    int c1 = computeCode3D(vp1.pos);


    std::vector<Vertex> result;

    bool run = true;


    while(run) {


        //both inside
        if(!(c0 | c1)) {
            run = false;
            break;
        } 
        //both in one of the areas defined above
        else if(c0 & c1) {
            vp0.visible = false;
            vp1.visible = false;
            run = false;
            break;


        }

        else {


            //at least one point is not inside. So pick largest bitcode to get it.
            int c = c1 > c0 ? c1 : c0;

            float t = 0.f;
            if (c & LEFT) {


                if (c == c0) t = L.lineSegmentIntersection(p0, p1);
                else  t = L.lineSegmentIntersection(p1, p0);

            }
            else if (c & RIGHT) {
                if (c == c0) t = R.lineSegmentIntersection(p0, p1);
                else  t = R.lineSegmentIntersection(p1, p0);
            }    
            else if (c & BOTTOM) {
                if (c == c0) t = BO.lineSegmentIntersection(p0, p1);
                else  t = BO.lineSegmentIntersection(p1, p0);
            }
            else if (c & TOP) {
                if (c == c0) t = T.lineSegmentIntersection(p0, p1);
                else  t = T.lineSegmentIntersection(p1, p0);
            }
            else if (c & FRONT) {
                if (c == c0) t = F.lineSegmentIntersection(p0, p1);
                else  t = F.lineSegmentIntersection(p1, p0);
            }
            else if (c & BACK) {
                if (c == c0) t = BA.lineSegmentIntersection(p0, p1);
                else  t = BA.lineSegmentIntersection(p1, p0);
            }


            if ( (t > 0.f ) && (t < 1.f)) {
            if (c == c0) {
                vp0.pos = vp0.pos + t * (vp1.pos - vp0.pos);
                vp0.color = vp0.color + t * (vp1.color - vp0.color);
                change0 = true;

                c0 = computeCode3D(vp0.pos);
            } else {
                vp1.pos = vp1.pos + t * (vp0.pos - vp1.pos);
                vp1.color = vp1.color + t * (vp0.color - vp1.color);
                change1 = true;

                c1 = computeCode3D(vp1.pos);
            }
            }
            else {break;}
        }
    }

    result.push_back(vp0);
    result.push_back(vp1);

    return result;
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


#endif

