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

    //left plane
    float ALeft = VP.c0.x + VP.c0.w;
    float BLeft = VP.c1.x + VP.c1.w;
    float CLeft = VP.c2.x + VP.c2.w;
    float DLeft = VP.c3.x + VP.c3.w;
    Plane LEFT = Plane(ALeft, BLeft, CLeft, DLeft);

    //right plane
    float ARight = -VP.c0.x + VP.c0.w;
    float BRight = -VP.c1.x + VP.c1.w;
    float CRight = -VP.c2.x + VP.c2.w;
    float DRight = -VP.c3.x + VP.c3.w;
    Plane RIGHT = Plane(ARight, BRight, CRight, DRight);

    //bottom plane
    float ABottom = VP.c0.y + VP.c0.w;
    float BBottom = VP.c1.y + VP.c1.w;
    float CBottom = VP.c2.y + VP.c2.w;
    float DBottom = VP.c3.y + VP.c3.w;
    Plane BOTTOM = Plane(ABottom, BBottom, CBottom, DBottom);

    //top plane
    float ATop = -VP.c0.y + VP.c0.w;
    float BTop = -VP.c1.y + VP.c1.w;
    float CTop = -VP.c2.y + VP.c2.w;
    float DTop = -VP.c3.y + VP.c3.w;
    Plane TOP = Plane(ATop, BTop, CTop, DTop);

    //front plane
    float AFront = VP.c0.z - VP.c0.w;
    float BFront = VP.c1.z - VP.c1.w;
    float CFront = VP.c2.z - VP.c2.w;
    float DFront = VP.c3.z - VP.c3.w;
    Plane FRONT = Plane(AFront, BFront, CFront, DFront);

    //Back plane
    float ABack = VP.c0.z + 2.f * VP.c0.w;
    float BBack = VP.c1.z + 2.f * VP.c1.w;
    float CBack = VP.c2.z + 2.f * VP.c2.w;
    float DBack = VP.c3.z + 2.f * VP.c3.w;
    Plane BACK = Plane(ABack, BBack, CBack, DBack);


    Vertex vps[3];

    vps[0].pos = pp0;
    vps[0].color = c0;
    vps[1].pos = pp1;
    vps[1].color = c1;
    vps[2].pos = pp2;
    vps[2].color = c2;

    std::vector<Vertex> result;
    bool changea0 = false;
    bool changea1 = false;
    bool changeb1 = false;
    bool changeb2 = false;
    bool changec2 = false;
    bool changec0 = false;

    std::vector<Vertex> resulta = cohenSutherlandClip3D(p0, p1, vps[0], vps[1], LEFT, RIGHT, BOTTOM, TOP, FRONT, BACK, changea0, changea1);
    if (changea0) result.push_back(resulta[0]);
    if (changea1) result.push_back(resulta[1]);

    std::vector<Vertex> resultb = cohenSutherlandClip3D(p1, p2, vps[1], vps[2], LEFT, RIGHT, BOTTOM, TOP, FRONT, BACK, changeb1, changeb2);
    if (changeb1) result.push_back(resultb[0]);
    if (changeb2) result.push_back(resultb[1]);

    std::vector<Vertex> resultc = cohenSutherlandClip3D(p2, p0, vps[2], vps[0], LEFT, RIGHT, BOTTOM, TOP, FRONT, BACK, changec2, changec0);
    if (changec2) result.push_back(resultc[0]);
    if (changec0) result.push_back(resultc[1]);


    if(!(resulta[0].visible | resultb[0].visible | resultc[0].visible)) {
        printf("THROW AWAY\n");
        return;
    } 
    if(result.size() > 4) { //corner case, for example can prpduce more than 4 vertices. ignore for now
        printf("THROW AWAY\n");
        return;
    } 

    if (!(changea0 | changec0)) result.push_back(vps[0]);
    if (!(changea1 | changeb1)) result.push_back(vps[1]);
    if (!(changeb2 | changec2)) result.push_back(vps[2]);

    int num_verts = result.size();
    Vertex pp [num_verts];
    for(int i = 0; i < num_verts; i++) {

        pp[i] = result[i]; 
        pp[i].pos = (1.f / pp[i].pos.w) * pp[i].pos;

        //printf("result: i: %i, x: %f, y: %f, z: %f, w: %f", i, pp[i].pos.x, pp[i].pos.y, pp[i].pos.z, pp[i].pos.w);
    }





    ///NDC space
    ///pp0 = (1.f / pp0.w) * pp0;
    ///pp1 = (1.f / pp1.w) * pp1;
    ///pp2 = (1.f / pp2.w) * pp2;

    //printf("pp0.x: %f, pp0.y: %f, pp0.z %f, pp0.w %f\n",pp0.x, pp0.y, pp0.z, pp0.w);
    //printf("pp1.x: %f, pp1.y: %f, pp1.z %f, pp1.w %f\n",pp1.x, pp1.y, pp1.z, pp1.w);
    //printf("pp2.x: %f, pp2.y: %f, pp2.z %f, pp2.w %f\n",pp2.x, pp2.y, pp2.z, pp2.w);


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

    for(int i = 0; i < num_verts; i++) {

        pp[i].pos = viewPort * pp[i].pos; 

    }

    Vec2i pi [num_verts];
    Vec4f c [num_verts];
    float zBuff [num_verts];

    for(int i = 0; i < num_verts; i++) {

        pi[i] = Vec2i((int)pp[i].pos.x, (int)pp[i].pos.y);
        zBuff[i] = pp[i].pos.z;

        c[i] = pp[i].color;

    }


    if(num_verts == 3) {
        drawTriangle(pi[0], pi[1], pi[2], c[0], c[1], c[2], zBuff[0], zBuff[1], zBuff[2], image);

    } else if (num_verts == 4) {

        drawQuad(pi, c, zBuff, image);


    }



}


















#endif
