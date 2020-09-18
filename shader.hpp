#ifndef SHADER_HPP
#define SHADER_HPP


#include "texture.hpp"
#include "image.hpp"
#include "vector.hpp"
#include "vertex.hpp"


extern Mat4f VP;

bool vertexShader(Vertex &v) {


    //clip space
    v.pos = VP * v.pos;

    return true;
}










bool fragmentShader(Vertex v0, Vertex v1, Vertex v2, Vec3f bary, int pixX, int pixY, Image* image) {


    //Color tex = Color(Vec4f(1.f, 0.f, 0.f, 1.f));
    checkerBoardTexture tex = checkerBoardTexture();

    float depth = bary.x * v0.pos.z + bary.y * v1.pos.z + bary.z * v2.pos.z;
    Vec3f n = bary.x * v0.normal + bary.y * v1.normal + bary.z * v2.normal;
    Vec2f uv = bary.x * v0.uv + bary.y * v1.uv + bary.z * v2.uv;


    Vec3f light = Vec3f(0.f, .707f, .707f);
    float l = Vec3dot(light, n);  // have to be normalized

    if( l < 0.f) {

        l = 0.f;
    }



    Vec3f colorf = l * 255.f * Vec4toVec3(tex.getColor(uv));
    Vec3i RGB = Vec3i((int)colorf.x, (int)colorf.y, (int)colorf.z);

    image->setPixel(pixX, pixY, RGB, depth);

    return true;
}















#endif
