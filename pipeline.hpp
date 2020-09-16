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


extern Mat4f view;
extern Mat4f persp;
extern Mat4f VP;
extern Mat4f viewPort;

void pipeline(Image* image, Vertex v0, Vertex v1, Vertex v2, Camera* cam) {

    Vec4f p0 = v0.pos;
    Vec4f p1 = v1.pos;
    Vec4f p2 = v2.pos;


    p0 = view * p0;  
    p1 = view * p1;  
    p2 = view * p2;  

    Vec3f l01 = Vec3f(p1.x, p1.y, p1.z) - Vec3f(p0.x, p0.y, p0.z);  
    Vec3f l02 = Vec3f(p2.x, p2.y, p2.z) - Vec3f(p0.x, p0.y, p0.z);  

    Vec3f n = Vec3cross(l01, l02);

    float dot = Vec3dot(n, cam->forward);

    if (dot > 0.f) {return;}


    //clip space
    Vec4f pp0 = VP * p0;
    Vec4f pp1 = VP * p1;
    Vec4f pp2 = VP * p2;


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


    //view port transform
    pp0 = viewPort * pp0; 
    pp1 = viewPort * pp1; 
    pp2 = viewPort * pp2; 


    v0.pos = pp0;    
    v1.pos = pp1;
    v2.pos = pp2;


    drawTriangle(v0, v1, v2, image);

    //drawLine(pi0, pi1, c0, c1, z0, z1, image);
    //drawLine(pi1, pi2, c1, c2, z1, z2, image);
    //drawLine(pi2, pi0, c2, c0, z2, z0, image);



}


















#endif
