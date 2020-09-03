#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include<stdio.h>

#include "camera.hpp"


void pipeline() {


vec3f p0 = vec3f(-0.5f, -0.5f, 0.f);
vec3f p1 = vec3f(0.5f, -0.5f, 0.f);
vec3f p2 = vec3f(0.0f, 0.5f, 0.f);


Camera cam = Camera();
printf("vy: %f\n",cam.v.y);











}


















#endif
