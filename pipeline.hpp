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
#include "shader.hpp"

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


extern Mat4f model, viewPort;

void pipeline(Image* image, Vertex v0, Vertex v1, Vertex v2, Camera* cam) {


    v0.pos = model * v0.pos;
    v1.pos = model * v1.pos;
    v2.pos = model * v2.pos;

    //back-face culling
    Vec3f l01 = Vec3f(v1.pos.x, v1.pos.y, v1.pos.z) - Vec3f(v0.pos.x, v0.pos.y, v0.pos.z);  
    Vec3f l02 = Vec3f(v2.pos.x, v2.pos.y, v2.pos.z) - Vec3f(v0.pos.x, v0.pos.y, v0.pos.z);  
    Vec3f n = Vec3cross(l01, l02);

    Vec3f cent = 0.33333333f * Vec4toVec3((v0.pos + v1.pos + v2.pos));

    float dot = Vec3dot(n, cent - cam->pos);

    if (dot > 0.f) {return;}


    vertexShader(v0);
    vertexShader(v1);
    vertexShader(v2);


    bool hidden01 = cohenSutherlandClip3D(v0.pos, v1.pos);
    bool hidden12 = cohenSutherlandClip3D(v1.pos, v2.pos);
    bool hidden20 = cohenSutherlandClip3D(v2.pos, v0.pos);

    if(hidden01 && hidden12 && hidden20) {
        return;
    } 


    //NDC transform (perspective divide)
    v0.pos = (1.f / v0.pos.w) * v0.pos; 
    v1.pos = (1.f / v1.pos.w) * v1.pos; 
    v2.pos = (1.f / v2.pos.w) * v2.pos; 


    //view port transform
    v0.pos = viewPort * v0.pos; 
    v1.pos = viewPort * v1.pos; 
    v2.pos = viewPort * v2.pos; 


    drawTriangle(v0, v1, v2, image);

    //drawLine(pi0, pi1, c0, c1, z0, z1, image);
    //drawLine(pi1, pi2, c1, c2, z1, z2, image);
    //drawLine(pi2, pi0, c2, c0, z2, z0, image);
}


















#endif
