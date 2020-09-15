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
 *
 *
 *viewport transformation -- now Z values are carried along in Z component again.
 *
 * |W/2    0      0    (W-1)/2|
 * |0      -H/2    0    (H-1)/2|
 * |0      0      1          0|
 * |0      0      0          1|
 */



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

    float n = -1.0f; //near plane
    float f = -2.f; //far plane
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


    //zDepths
    float z0 = pp0.w;
    float z1 = pp1.w;
    float z2 = pp2.w;

    bool hidden01 = cohenSutherlandClip3D(pp0, pp1);
    bool hidden12 = cohenSutherlandClip3D(pp1, pp2);
    bool hidden20 = cohenSutherlandClip3D(pp2, pp0);

    if(hidden01 && hidden12 && hidden20) {
        return;
    } 


    //NDC transform
    pp0 = (1.f / pp0.w) * pp0; 
    pp1 = (1.f / pp1.w) * pp1; 
    pp2 = (1.f / pp2.w) * pp2; 


    //Viewport transform
    Mat4f viewPort = Mat4f();
    viewPort.c0 = Vec4f(SCREEN_WIDTH / 2.f, 0.f, 0.f, 0.f);
    viewPort.c1 = Vec4f(0.f, -SCREEN_HEIGHT / 2.f, 0.f, 0.f);
    viewPort.c2 = Vec4f(0.f, 0.f, 1.f, 0.f);
    viewPort.c3 = Vec4f((SCREEN_WIDTH - 1.f) / 2.f, (SCREEN_HEIGHT - 1.f) / 2.f, 0.f, 1.f);

    pp0 = viewPort * pp0; 
    pp1 = viewPort * pp1; 
    pp2 = viewPort * pp2; 

    Vec2i pi0 = Vec2i((int)pp0.x, (int)pp0.y);
    Vec2i pi1 = Vec2i((int)pp1.x, (int)pp1.y);
    Vec2i pi2 = Vec2i((int)pp2.x, (int)pp2.y);

    //printf("pi0.x: %i, pi0.y: %i\n", pi0.x, pi0.y);
    //printf("pi1.x: %i, pi1.y: %i\n", pi1.x, pi1.y);
    //printf("pi2.x: %i, pi2.y: %i\n", pi2.x, pi2.y);

    //drawTriangle(pi0, pi1, pi2, c0, c1, c2, z0, z1, z2, image);

    drawLine(pi0, pi1, c0, c1, z0, z1, image);
    drawLine(pi1, pi2, c1, c2, z1, z2, image);
    drawLine(pi2, pi0, c2, c0, z2, z0, image);



}


















#endif
