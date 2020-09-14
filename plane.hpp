#ifndef PLANE_HPP
#define PLANE_HPP

#include "vector.hpp"
#include "vertex.hpp"





/* General Form: Ax + By + Cz + D = 0
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
 * and since W = 1 always,
 *
 * A * X + B * Y + C * Z + D = 0
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



    float evaluatePlane(Vec4f p) {


    return A * p.x + B * p.y + C * p.z + D;

    }



    float lineSegmentIntersection(Vec4f p0, Vec4f p1) {

    Vec3f a = Vec3f(p0.x, p0.y, p0.z);
    Vec3f b = Vec3f(p1.x, p1.y, p1.z);
    Vec3f n = Vec3f(A, B, C);


    float npdota = Vec3dot(n,a);
    float npdotabdif = Vec3dot(n, a - b);


    if (npdotabdif == 0.f) return -1000.f;
    else return  (npdota + D) /npdotabdif;

    }







};













#endif
