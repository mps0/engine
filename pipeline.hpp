#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include<stdio.h>

#include "camera.hpp"
#include "matrix.hpp"
#include "vector.hpp"


void pipeline(Image* image, Vec4f p0, Vec4f p1, Vec4f p2) {


    Camera cam = Camera();

    Mat4f rot = Mat4f();
    rot.c0 = Vec4f(cam.u.x, cam.v.x, cam.w.x, 0.f);
    rot.c1 = Vec4f(cam.u.y, cam.v.y, cam.w.y, 0.f);
    rot.c2 = Vec4f(cam.u.z, cam.v.z, cam.w.z, 0.f);
    rot.c3 = Vec4f(0.f, 0.f, 0.f, 1.f);


    Mat4f trans = Mat4f();
    trans.c0 = Vec4f(1.f, 0.f, 0.f, 0.f);
    trans.c1 = Vec4f(0.f, 1.f, 0.f, 0.f);
    trans.c2 = Vec4f(0.f, 0.f, 1.f, 0.f);
    trans.c3 = Vec4f(-cam.pos.x, -cam.pos.y, -cam.pos.z, 1.f);


    Mat4f view = rot * trans;


    float d = 1.f;

    Mat4f persp = Mat4f();
    persp.c0 = Vec4f(d, 0.f, 0.f, 0.f);
    persp.c1 = Vec4f(0.f, d, 0.f, 0.f);
    persp.c2 = Vec4f(0.f, 0.f, d, -1.f);
    persp.c3 = Vec4f(0.f, 0.f, 0.f, 0.f);

    Mat4f VP = persp * view;

    Vec4f pp0 = VP * p0;
    Vec4f pp1 = VP * p1;
    Vec4f pp2 = VP * p2;

    pp0 = (1.f / pp0.w) * pp0;
    pp1 = (1.f / pp1.w) * pp1;
    pp2 = (1.f / pp2.w) * pp2;


    Mat3f viewPort = Mat3f();
    viewPort.c0 = Vec3f(640.f / 2.f, 0.f, 0.f);
    viewPort.c1 = Vec3f(0.f, -480.f / 2.f, 0.f);
    viewPort.c2 = Vec3f((640.f-1.f) / 2.f, (480.f - 1.f) / 2.f, 1.f);

    Vec3f pvp0 = Vec3f(pp0.x, pp0.y, 1.f);
    Vec3f pvp1 = Vec3f(pp1.x, pp1.y, 1.f);
    Vec3f pvp2 = Vec3f(pp2.x, pp2.y, 1.f);


    Vec3f ppvp0 = viewPort * pvp0;
    Vec3f ppvp1 = viewPort * pvp1;
    Vec3f ppvp2 = viewPort * pvp2;

    Vec2i pi0 = Vec2i((int)ppvp0.x, (int)ppvp0.y);
    Vec2i pi1 = Vec2i((int)ppvp1.x, (int)ppvp1.y);
    Vec2i pi2 = Vec2i((int)ppvp2.x, (int)ppvp2.y);


    drawLines(pi0, pi1, image);
    drawLines(pi1, pi2, image);
    drawLines(pi2, pi0, image);










}


















#endif
