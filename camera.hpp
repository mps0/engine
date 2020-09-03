#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "vector.hpp"


class Camera{
    public:
      vec3f up;
      vec3f forward;
      vec3f pos; 

     Camera() : up(vec3f(0.f, 1.f, 0.1)), forward(vec3f(0.f, 0.f, 1.f)), pos(vec3f(0.f, 0.f, -1.f)) {}

     Camera(up, forward, pos) : up(up), forward(forward), pos(pos) {}

     vec3f w = -forward.normalize();
     vec3f u = vec3cross(up, w).normalize();
     vec3f v = vec3cross(w, u);


};

#endif
