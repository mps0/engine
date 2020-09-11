#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "vector.hpp"
#include "matrix.hpp"

//debug
#include<stdio.h>

class Camera{
    public:

        Vec3f up, forward, pos;
//       Camera() : up(Vec3f(0.f, 1.f, 0.f)), forward(Vec3f(0.f, 0.f, -1.f)), pos(Vec3f(0.f, 0.f, 0.f)) {}

       Camera() : up(Vec3f(0.f, 1.f, 0.f)), forward(Vec3f(0.f, 0.f, -1.f)), pos(Vec3f(-.5f, 0.f, 0.f)) {}


        Camera(Vec3f up, Vec3f forward, Vec3f pos) : up(up), forward(forward), pos(pos) {}

        Vec3f w = -forward.normalize();
        Vec3f u = Vec3cross(up, w).normalize();
        Vec3f v = Vec3cross(w, u);
};

#endif
