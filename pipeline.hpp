#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <stdio.h>
#include <algorithm>

#include "camera.hpp"
#include "matrix.hpp"
#include "vector.hpp"
#include "drawTriangle.hpp"
#include "drawLine.hpp"
#include "plane.hpp"
#include "vertex.hpp"
#include "clip.hpp"


void pipeline(Image* image, Vertex v0, Vertex v1, Vertex v2, Camera* cam) {

    Vec4f c0 = v0.color;
    Vec4f c1 = v1.color;
    Vec4f c2 = v2.color;

    Vec4f p0 = v0.pos;
    Vec4f p1 = v1.pos;
    Vec4f p2 = v2.pos;

    //transform from world-space to camera space
    //first match rotation
    Mat4f rot = Mat4f();
    rot.c0 = Vec4f(cam->u.x, cam->v.x, cam->w.x, 0.f);
    rot.c1 = Vec4f(cam->u.y, cam->v.y, cam->w.y, 0.f);
    rot.c2 = Vec4f(cam->u.z, cam->v.z, cam->w.z, 0.f);
    rot.c3 = Vec4f(0.f, 0.f, 0.f, 1.f);
    //now, line up origins
    Mat4f trans = Mat4f();
    trans.c0 = Vec4f(1.f, 0.f, 0.f, 0.f);
    trans.c1 = Vec4f(0.f, 1.f, 0.f, 0.f);
    trans.c2 = Vec4f(0.f, 0.f, 1.f, 0.f);
    trans.c3 = Vec4f(-cam->pos.x, -cam->pos.y, -cam->pos.z, 1.f);
    //combine
    Mat4f view = rot * trans;


    //perspective transform, withwith n = near plane, and f = far plane
    //
    /* From similar triangles, 
     * Xp/n = X/Z
     * Xp = n/Z * X
     *
     * Yp/n = Y/Z
     * Yp = n/Z * Y
     *
     * Zp = n
     *
     * Want to get rid of nonlinearly from division by Z to keep matrices affine. So
     * multiply all coordinates by Z, and then get the correct values when performing 
     * the transform from homogenous coordinates.
     *
     * Xp' = n * X
     * Yp' = n * Y
     *
     * Z is a little complicated. We want Z = n, to map to Zp = n, and Z = f to map
     * to Zp = f. After transformation the Zp values are meaningless other than to let
     * us know if we are in the view frustum volume. The old Z values are stored in
     * W-component for the homogenous transform.
     *
     * Zp = (-f * n) /z + f + n
     *
     * and
     * 
     * Zp' = -f * n + (f + n) * Z
     *
     * Thus, the perpsective matrix is:
     *
     * |n   0    0    0 |
     * |0   n    0    0 |
     * |0   0   f+n -f*n|
     * |0   0    1    0 |
     */

    float n = -1.f;
    float f = -2.f;
    Mat4f persp = Mat4f();
    persp.c0 = Vec4f(n, 0.f, 0.f, 0.f);
    persp.c1 = Vec4f(0.f, n, 0.f, 0.f);
    persp.c2 = Vec4f(0.f, 0.f, f + n, 1);
    persp.c3 = Vec4f(0.f, 0.f, -f * n, 0.f);

    Mat4f VP = persp * view;

    //clip space
    Vec4f pp0 = VP * p0;
    Vec4f pp1 = VP * p1;
    Vec4f pp2 = VP * p2;


    Vertex vps[3];
    vps[0].pos = pp0;
    vps[0].color = c0;
    vps[1].pos = pp1;
    vps[1].color = c1;
    vps[2].pos = pp2;
    vps[2].color = c2;

   bool hidden01 = cohenSutherlandClip3D(vps[0].pos, vps[1].pos);
   bool hidden12 = cohenSutherlandClip3D(vps[1].pos, vps[2].pos);
   bool hidden20 = cohenSutherlandClip3D(vps[2].pos, vps[0].pos);

    if(hidden01 && hidden12 && hidden20) {
        printf("THROW AWAY\n");
        return;
    } 


    //NDC transform
    Vertex pp [3];
    for(int i = 0; i < 3; i++) {
        pp[i] = vps[i]; 
        pp[i].pos = (1.f / pp[i].pos.w) * pp[i].pos;
        printf("pp.x: %f\n",pp[i].pos.x);
    }


    //viewport transformation -- now Z values are carried along in Z component again.
    /*
     * |W/2    0      0    (W-1)/2|
     * |0      -H/2    0    (H-1)/2|
     * |0      0      1          0|
     * |0      0      0          1|
     */


    Mat4f viewPort = Mat4f();
    viewPort.c0 = Vec4f(SCREEN_WIDTH / 2.f, 0.f, 0.f, 0.f);
    viewPort.c1 = Vec4f(0.f, -SCREEN_HEIGHT / 2.f, 0.f, 0.f);
    viewPort.c2 = Vec4f(0.f, 0.f, 1.f, 0.f);
    viewPort.c3 = Vec4f((SCREEN_WIDTH - 1.f) / 2.f, (SCREEN_HEIGHT - 1.f) / 2.f, 0.f, 1.f);

    for(int i = 0; i < 3; i++) {

        pp[i].pos = viewPort * pp[i].pos; 
    }

    Vec2i pi [3];
    Vec4f c [3];
    float zBuff [3];

    for(int i = 0; i < 3; i++) {

        printf("pp.x: %f, pp.z %f\n",pp[i].pos.x, pp[i].pos.z);
        pi[i] = Vec2i((int)pp[i].pos.x, (int)pp[i].pos.y);
        zBuff[i] = pp[i].pos.z;
        c[i] = pp[i].color;
    }


        drawTriangle(pi[0], pi[1], pi[2], c[0], c[1], c[2], zBuff[0], zBuff[1], zBuff[2], image);




}


















#endif
