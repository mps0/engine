#ifndef PLANE_HPP
#define PLANE_HPP

#include "pipeline.hpp"
#include "vector.hpp"
#include "vertex.hpp"





/* General Form: Ax + By + Cd + D = 0
 *
 * Implicit Plane: f(p) = dot(n, p - a) = 0
 *
 * Implicit Line: f(p) = a + t * (b - a) = 0
 *
 * Want to find where the triangle segment intersectcs plane.
 *
 *
 * f(p) = dot(n, p - a) = dot(n, p) - dot(n, a) = dot(n ,p) + D
 *      = n.x * p.x + n.y * p.y + n.z * p.z + D
 * So,
 *
 * (A, B, C) = n, D = - dot(n, a)
 *
 * Line segment intersection with plane:
 *
 * dot(n, p) + D = dot(n, (a + t * (b - a)) + D
 *               = dot(n, a) + t * dot(n, b - a) + D 
 *
 * Solving for t,
 *
 * t = (-dot(n,a) - D) / dot(n, b - a) 
 *   = (dot(n,a) + D) / dot(n, a - b)
 *
 *
 * Construction view frustum planes:
 *
 * Consider the x coordinate clipping in NDC:
 *
 *   -1 < x < 1
 *
 * Since NDC space is just clip space after the perspective transform,
 *   -x' < x' < w'
 *
 * Consider just left plane cliipping,
 *
 *  -w' < x'   ---->  x' + w' > 0
 *
 * x' = MVP * X;
 * w' = MVP * W;
 *
 *
 *       |00  01   02   03|    | X  |   | x'  |
 *       |10  11   12   13|    | Y  |   | y'  |
 *       |20  21   22   23| *  | Z  | = | z'  |    
 *       |30  31   32   33|    | W  |   | w'  |
 *
 *
 * So The plane is,
 * A * X + B * Y + C * Z + D * W = 0
 *
 * Where,
 * 
 * A = (m00 + m30)
 * B = (m01 + m31)
 * C = (m02 + m32)
 * D = (m03 + m33)
 * 
 *
 */


class Plane{
public:
    float A, B, C, D;

    Plane(float A, float B, float C, float D) : A(A), B(B), C(C), D(D) {}




    Vertex* triangleIntersection(Vertex v0, Vertex v1, Vertex v2, Vertex vp0, Vertex vp1, Vertex vp2) {

    static Vertex rVal [3];
    Vertex verts [3] = {v0, v1, v2};
    Vertex vertsP [3] = {vp0, vp1, vp2};


        Vec3f n = Vec3f(A, B, C);

    int next = 1;
    for(int i = 0; i < 3; i++) {


    Vec4f a = verts[i].pos;

    Vec4f  b = verts[next].pos;


    //printf("i: %i, next: %i\n", i, next);

    float f = A * a.x + B * a.y + C * a.z + D ;

    if ((f > 0) && (i != 1)) {

        float npdota = Vec3dot(n,Vec3f(a.x, a.y, a.z));
        float npdotabdif = Vec3dot(n, (Vec3f(a.x, a.y, a.z) - Vec3f(b.x, b.y, b.z)));

        float t = (npdota + D) /npdotabdif;

        rVal[i].pos = vertsP[i].pos + t * (vertsP[next].pos - vertsP[i].pos);
        rVal[i].color = vertsP[i].color + t * (vertsP[next].color - vertsP[i].color);
        //pp0 = pp0 + t * (pp1 - pp0);
        //c0 = c0 + t * (c1 - c0);

    }
    

    else {

        rVal[i] = vertsP[i];

    }

    //next = (next + 1) % 3;
    }


       
    return rVal;

    }


    //segmentIntersection(Vec3f p0, Vec3f p1) {





    //}
    



















};













#endif
