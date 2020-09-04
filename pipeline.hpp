#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include<stdio.h>

#include "camera.hpp"
#include "matrix.hpp"
#include "vector.hpp"


void pipeline() {


Vec3f p0 = Vec3f(-0.5f, -0.5f, 0.f);
Vec3f p1 = Vec3f(0.5f, -0.5f, 0.f);
Vec3f p2 = Vec3f(0.0f, 0.5f, 0.f);

Camera cam = Camera();


        Mat4f rot = Mat4f();
        rot.c0 = Vec4f(cam.u.x, cam.v.x, cam.w.x, 0.f);
        rot.c1 = Vec4f(cam.u.y, cam.v.y, cam.w.y, 0.f);
        rot.c2 = Vec4f(cam.u.z, cam.v.z, cam.w.z, 0.f);
        rot.c3 = Vec4f(0.f, 0.f, 0.f, 1.f);


        Mat4f trans = Mat4f();
        trans.c0 = Vec4f(1.f, 0., 0.f, 0.f);
        trans.c1 = Vec4f(0.f, 1., 0.f, 0.f);
        trans.c2 = Vec4f(0.f, 0., 1.f, 0.f);
        trans.c3 = Vec4f(-cam.pos.x, -cam.pos.y, -cam.pos.z, 1.f);

        Mat4f view = rot * trans;

        printf("c3w: %f\n",view.c3.w);








}


















#endif
