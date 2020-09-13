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




    Vertex vs [3];

    vs[0] = v0;
    vs[1] = v1;
    vs[2] = v2;

    Vertex vps[3];

    vps[0].pos = pp0;
    vps[0].color = c0;
    vps[1].pos = pp1;
    vps[1].color = c1;
    vps[2].pos = pp2;
    vps[2].color = c2;




    //for each plane

    std::vector<Vertex> out;
    std::vector<Vertex> in;

    std::vector<Vertex> outP;
    std::vector<Vertex> inP;

    std::vector<Vertex> result;

    for(int i = 0; i < 3; i++) { 
        float f =  LEFT.evaluatePlane(vs[i].pos);

        printf("f: %f\n", f);
        if (f > 0) {

            out.push_back(vs[i]);
            outP.push_back(vps[i]);
        }

        else {
        printf("i: %i\n",i);

            in.push_back(vs[i]);
            inP.push_back(vps[i]);
        }
    } 
    printf("out.size(): %i, in.size(): %i\n", out.size(), in.size());



    if (out.size() == 3){

        //do nothing
    }

    else if(out.size() == 0) {

    result = inP; 

    }


    else if (out.size() == 2) {

        for (int i = 0; i < out.size(); i++) {
            for (int j = 0; j < in.size(); j++) {


                float t = LEFT.lineSegmentIntersection(out[i].pos, in[j].pos);
                Vertex vpn;
                vpn.pos = outP[i].pos + t * (inP[j].pos - outP[i].pos);
                vpn.color = outP[i].color + t * (inP[j].color - outP[i].color);
                result.push_back(vpn);
            }
        }

        for (int i = out.size(); i < (in.size() + out.size()); i++){
            //printf("i: %i\n",i);

            Vertex vpn;
            vpn = inP[i - out.size()];
            result.push_back(vpn);
        }
    }

    else if (out.size() == 1) {

        //for (int j = 0; j < in.size(); j++) {
        //    float t = LEFT.lineSegmentIntersection(out[0].pos, in[j].pos);
        //    Vertex vpn;
        //    vpn.pos = outP[0].pos + t * (inP[j].pos - outP[0].pos);
        //    vpn.color = outP[0].color + t * (inP[j].color - outP[0].color);
        //    result.push_back(vpn);
        //    result.push_back(inP[j]);
        //}
        
        float t0 = LEFT.lineSegmentIntersection(out[0].pos, in[0].pos);
        float t1 = LEFT.lineSegmentIntersection(out[0].pos, in[1].pos);
        Vertex vpn0, vpn1;
        vpn0.pos = outP[0].pos + t0 * (inP[0].pos - outP[0].pos);
        vpn0.color = outP[0].color + t0 * (inP[0].color - outP[0].color);
        vpn1.pos = outP[0].pos + t1 * (inP[1].pos - outP[0].pos);
        vpn1.color = outP[0].color + t1 * (inP[1].color - outP[0].color);

        if (vpn1.pos.y > vpn0.pos.y) { 
        
            result.push_back(vpn1);
            result.push_back(vpn0);


        } 
        else {


            result.push_back(vpn0);
            result.push_back(vpn1);
        }

        if (inP[1].pos.y > inP[0].pos.y) { 
        
            result.push_back(inP[1]);
            result.push_back(inP[0]);


        } 
        else {
            result.push_back(inP[0]);
            result.push_back(inP[1]);


        }


        //result.push_back(vpn);
        //result.push_back(inP[j]);


    }



   int num_verts = result.size();

   //pp0 = vps[0].pos;
   //c0 = vps[0].color;
   //pp1 = vps[1].pos;
   //c1 = vps[1].color;
   //pp2 = vps[2].pos;
   //c2 = vps[2].color;

    
   Vertex pp [num_verts];

   for(int i = 0; i < num_verts; i++) {

    pp[i] = result[i]; 
    pp[i].pos = (1.f / pp[i].pos.w) * pp[i].pos;

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


    //Vec4f ppvp0 = viewPort * pp0;
    //Vec4f ppvp1 = viewPort * pp1;
    //Vec4f ppvp2 = viewPort * pp2;

    //printf("ppvp0.x: %f, ppvp0.y: %f, ppvp0.z %f, ppvp0.w %f\n",ppvp0.x, ppvp0.y, ppvp0.z, ppvp0.w);
    //printf("ppvp1.x: %f, ppvp1.y: %f, ppvp1.z %f, ppvp1.w %f\n",ppvp1.x, ppvp1.y, ppvp1.z, ppvp1.w);
    //printf("ppvp2.x: %f, ppvp2.y: %f, ppvp2.z %f, ppvp2.w %f\n",ppvp2.x, ppvp2.y, ppvp2.z, ppvp2.w);

    //Vec2i pi0 = Vec2i((int)ppvp0.x, (int)ppvp0.y);
    //Vec2i pi1 = Vec2i((int)ppvp1.x, (int)ppvp1.y);
    //Vec2i pi2 = Vec2i((int)ppvp2.x, (int)ppvp2.y);

    ////save depths for depth buffer
    //float z0 = ppvp0.z;
    //float z1 = ppvp1.z;
    //float z2 = ppvp2.z;

    

    //drawLine(pi0, pi1, c0, c1, z0, z1, image);
    //drawLine(pi1, pi2, c1, c2, z1, z2, image);
    //drawLine(pi2, pi0, c2, c0, z2, z0, image);


    //drawTriangle(pi0, pi1, pi2, c0, c1, c2, z0, z1, z2, image);
    //
    if(num_verts == 3) {
    drawTriangle(pi[0], pi[1], pi[2], c[0], c[1], c[2], zBuff[0], zBuff[1], zBuff[2], image);

    } else if (num_verts == 4) {

        printf("hi\n");

 //   drawQuad(pi[0], pi[1], pi[2], pi[3], c[0], c[1], c[2], c[3], zBuff[0], zBuff[1], zBuff[2], zBuff[3], image);
    drawQuad(pi, c, zBuff, image);


    }



    //        printf("pi0.x: %i, pi0.y : %i, p1.0x : %i, p1.y: %i, p2.x: %i, p2.y: %i\n",pi0.x, pi0.y, pi1.x, pi1.y, pi2.x, pi2.y);
    //
}


















#endif
