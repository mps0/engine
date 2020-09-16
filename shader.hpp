#ifndef SHADER_HPP
#define SHADER_HPP


#include "image.hpp"
#include "vector.hpp"
#include "vertex.hpp"



//Vertex vShader(Vertex &v) {
//
//
//
//
//
//
//
//
//
//
//
//
//}
//
//


void fragmentShader(Vertex v0, Vertex v1, Vertex v2, Vec2i pi, Vec3f bary, Image* image) {



    float depth = bary.x * v0.pos.z + bary.y * v1.pos.z + bary.z * v2.pos.z;
    Vec3f n = bary.x * v0.normal + bary.y * v1.normal + bary.z * v2.normal;


    Vec3f light = Vec3f(0.f, .707f, .707f);
    float l = Vec3dot(light, n);  // have to be normalized

    if( l < 0.f) {

        l = 0.f;
    }

    Vec3f colorf = l * Vec3f(255.f, 255.f, 255.f);
    Vec3i RGB = Vec3i((int)colorf.x, (int)colorf.y, (int)colorf.z);

    image->setPixel(pi.x, pi.y, RGB, depth);






}















#endif
