#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include<stdio.h>

#include "camera.hpp"
#include "matrix.hpp"
#include "vector.hpp"
#include "drawTriangle.hpp"
#include "drawLine.hpp"
#include "plane.hpp"
#include "vertex.hpp"


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


    //printf("p0.x: %f, p0.y: %f, p0.z %f, p0.w %f\n",p0.x, p0.y, p0.z, p0.w);
    //printf("p1.x: %f, p1.y: %f, p1.z %f, p1.w %f\n",p1.x, p1.y, p1.z, p1.w);
    //printf("p2.x: %f, p2.y: %f, p2.z %f, p2.w %f\n",p2.x, p2.y, p2.z, p2.w);
    //
    //printf("pp0.x: %f, pp0.y: %f, pp0.z %f, pp0.w %f\n",pp0.x, pp0.y, pp0.z, pp0.w);
    //printf("pp1.x: %f, pp1.y: %f, pp1.z %f, pp1.w %f\n",pp1.x, pp1.y, pp1.z, pp1.w);
    //printf("pp2.x: %f, pp2.y: %f, pp2.z %f, pp2.w %f\n",pp2.x, pp2.y, pp2.z, pp2.w);

    //printf("np.x: %f, np.y: %f, np.z: %f)\n",np.x, np.y, np.z);
    

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

    
    Vertex vpp0, vpp1, vpp2;

    vpp0.pos = pp0;
    vpp0.color = v0.color;

    vpp1.pos = pp1;
    vpp1.color = v1.color;

    vpp2.pos = pp2;
    vpp2.color = v2.color;

    Vertex* newTris = LEFT.triangleIntersection(v0, v1, v2, vpp0, vpp1, vpp2);

    pp0 = newTris[0].pos;
    c0 = newTris[0].color;

    pp1 = newTris[1].pos;
    c1 = newTris[1].color;

    pp2 = newTris[2].pos;
    c2 = newTris[2].color;

    //printf("pp0.x: %f, pp0.y: %f, pp0.z %f, pp0.w %f\n",pp0.x, pp0.y, pp0.z, pp0.w);


    //Vec3f npLeft = Vec3f(ALeft, BLeft, CLeft);
    //
    //printf("A: %f, B: %f, C: %f, D: %f\n", ALeft, BLeft, CLeft, DLeft);



    //printf("pp0.x: %f, pp0.y: %f, pp0.z %f, pp0.w %f\n",pp0.x, pp0.y, pp0.z, pp0.w);
    //float fLeftp0 = ALeft * p0.x + BLeft * p0.y + CLeft * p0.z + DLeft;
    //printf("fLeftp0: %f\n",fLeftp0);


    //if (fLeftp0 > 0) {

    //    float npDota = Vec3dot(npLeft,Vec3f(p0.x, p0.y, p0.z));
    //    float npDotabdif = Vec3dot(npLeft, (Vec3f(p0.x, p0.y, p0.z) - Vec3f(p1.x, p1.y, p1.z)));

    //    float t = (npDota + DLeft) /npDotabdif;

    //    printf("t: %f\n",t);
    //    //p0 = p0 + t * (p1 - p0);
    //    //pp0 = VP * p0;

    //    pp0 = pp0 + t * (pp1 - pp0);
    //    c0 = c0 + t * (c1 - c0);

    //}
    //float fLeftp2 = ALeft * p2.x + BLeft * p2.y + CLeft * p2.z + DLeft;

    //if (fLeftp2 > 0) {

    //    float npDota = Vec3dot(npLeft,Vec3f(p2.x, p2.y, p2.z));
    //    float npDotabdif = Vec3dot(npLeft, (Vec3f(p2.x, p2.y, p2.z) - Vec3f(p1.x, p1.y, p1.z)));

    //    float t = (npDota + DLeft) /npDotabdif;

    //    printf("t: %f\n",t);
    //    //p0 = p0 + t * (p1 - p0);
    //    //pp0 = VP * p0;

    //    pp2 = pp2 + t * (pp1 - pp2);
    //    c2 = c2 + t * (c1 - c2);

    //}





    //NDC space
    pp0 = (1.f / pp0.w) * pp0;
    pp1 = (1.f / pp1.w) * pp1;
    pp2 = (1.f / pp2.w) * pp2;

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


    Vec4f ppvp0 = viewPort * pp0;
    Vec4f ppvp1 = viewPort * pp1;
    Vec4f ppvp2 = viewPort * pp2;

    //printf("ppvp0.x: %f, ppvp0.y: %f, ppvp0.z %f, ppvp0.w %f\n",ppvp0.x, ppvp0.y, ppvp0.z, ppvp0.w);
    //printf("ppvp1.x: %f, ppvp1.y: %f, ppvp1.z %f, ppvp1.w %f\n",ppvp1.x, ppvp1.y, ppvp1.z, ppvp1.w);
    //printf("ppvp2.x: %f, ppvp2.y: %f, ppvp2.z %f, ppvp2.w %f\n",ppvp2.x, ppvp2.y, ppvp2.z, ppvp2.w);

    Vec2i pi0 = Vec2i((int)ppvp0.x, (int)ppvp0.y);
    Vec2i pi1 = Vec2i((int)ppvp1.x, (int)ppvp1.y);
    Vec2i pi2 = Vec2i((int)ppvp2.x, (int)ppvp2.y);

    //save depths for depth buffer
    float z0 = ppvp0.z;
    float z1 = ppvp1.z;
    float z2 = ppvp2.z;

    

    //drawLine(pi0, pi1, c0, c1, z0, z1, image);
    //drawLine(pi1, pi2, c1, c2, z1, z2, image);
    //drawLine(pi2, pi0, c2, c0, z2, z0, image);

    drawTriangle(pi0, pi1, pi2, c0, c1, c2, z0, z1, z2, image);



    //        printf("pi0.x: %i, pi0.y : %i, p1.0x : %i, p1.y: %i, p2.x: %i, p2.y: %i\n",pi0.x, pi0.y, pi1.x, pi1.y, pi2.x, pi2.y);
    //
}


















#endif
