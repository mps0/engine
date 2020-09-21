#ifndef SHADER_HPP
#define SHADER_HPP


#include "texture.hpp"
#include "image.hpp"
#include "vector.hpp"
#include "vertex.hpp"
#include "light.hpp"


extern Mat4f VP;

bool vertexShader(Vertex &v) {

    //clip space
    v.pos = VP * v.pos;

    return true;
}










bool fragmentShader(Vertex v0, Vertex v1, Vertex v2, Vec3f bary, int pixX, int pixY, Texture* tex, Image* image, bool s) {

    float depth = bary.x * v0.pos.z + bary.y * v1.pos.z + bary.z * v2.pos.z;
    Vec3f n = bary.x * v0.normal + bary.y * v1.normal + bary.z * v2.normal;
    Vec2f uv = bary.x * v0.uv + bary.y * v1.uv + bary.z * v2.uv;


    float l = 1.f;
    Vec3i RGB;
    if (!s){
    ShadowLight light;
    l = light.getLight(v0.worldPos, v1.worldPos, v2.worldPos, bary);
    }

    l = 15.f * l;
    if(l > 1.f) l = 1.f;
    else if (l < 0.f)  l = 0.f;


    //Vec3f light = Vec3f(0.f, .707f, .707f);
    //float l = Vec3dot(light, n);  // have to be normalized

    //if( l < 0.f) {

    //    l = 0.f;
    //}
    //



    Vec3f colorf = l * 255.f * Vec4toVec3(tex->getColor(uv));
    //Vec3f colorf = 255.f * Vec4toVec3(tex->getColor(uv));
    RGB = Vec3i((int)colorf.x, (int)colorf.y, (int)colorf.z);


    image->setPixel(pixX, pixY, RGB, depth);

    return true;
}















#endif
