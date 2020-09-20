#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "image.hpp"
#include "vector.hpp"


extern ShadowMap* sm;
extern Mat4f VPShadow, viewPort;

class ShadowLight{
    public:




  float  getLight(Vec4f p0, Vec4f p1, Vec4f p2, Vec3f bary) {


   //printf("p0.x: %f\n",p0.x);
        
   p0 = VPShadow * p0;
   p1 = VPShadow * p1;
   p2 = VPShadow * p2;

   p0 = (1.f / p0.w) * p0;
   p1 = (1.f / p1.w) * p1;
   p2 = (1.f / p2.w) * p2;

   p0 = viewPort * p0;
   p1 = viewPort * p1;
   p2 = viewPort * p2;

   float zDepth = bary.x * p0.z + bary.y * p1.z + bary.z * p2.z;


   float x0 = p0.x;
   float y0 = p0.y;

   float x1 = p1.x;
   float y1 = p1.y;

   float x2 = p2.x;
   float y2 = p2.y;


   //printf("X0: %f\n",x0);

   if ((x0 < 720.f) && (x0 >=0.f)){
   if ((y0 < 720.f) && (y0 >=0.f)){

   if ((x1 < 720.f) && (x1 >=0.f)){
   if ((y1 < 720.f) && (y1 >=0.f)){
   
   if ((x2 < 720.f) && (x2 >=0.f)){
   if ((y2 < 720.f) && (y2 >=0.f)){

   int x = (int)(bary.x * x0 + bary.y * x1 + bary.z * x2);     
   int y = (int)(bary.x * y0 + bary.y * y1 + bary.z * y2);     

   if (x < 0 || y < 0) {
       return 0.f;
   }


   //printf("x: %i, y: %i\n", x, y);
   return sm->getMap(x,y, zDepth);
   //return 1.f;


    
   }}}}}}

   //printf("SHADOW\n");

   return 0.f;
  }


};






























#endif
