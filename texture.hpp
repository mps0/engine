#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "vector.hpp"


class Texture{
    public:

    Vec3f U,V, origin;
    Vec2f scale;


    virtual Vec4f getColor(Vec2f uv) = 0;




    




};


class Color: public Texture{
    public:

    Vec4f color;


    Color(Vec4f color) : color(color) {}

    virtual Vec4f getColor(Vec2f uv)  {

        return color;



    }



    


};


class checkerBoardTexture: public Texture{
    public:
    float scale = 5.f;
     


    virtual Vec4f getColor(Vec2f uv) {

    Vec2i uvi = Vec2i((int)(1.999999f * scale * uv.x), (int)(1.999999f * scale * uv.y));

    int umod2 =  uvi.x % 2;
    int vmod2 =  uvi.y % 2;

    if (umod2 == vmod2) 
    return Vec4f(1.f, 1.f, 1.f, 1.f);
    else return Vec4f(0.f, 0.f, 1.f, 1.f);
    
    }









};

















#endif
