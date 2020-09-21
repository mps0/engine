#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "image.hpp"
#include "vector.hpp"


extern ShadowMap* sm;
extern Mat4f VPShadow, viewPort;

class ShadowLight{
    public:




  float  getLight(Vec4f p0, Vec4f p1, Vec4f p2, Vec3f bary) {

        
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


   int x = (int)(bary.x * p0.x + bary.y * p1.x + bary.z * p2.x);     
   int y = (int)(bary.x * p0.y + bary.y * p1.y + bary.z * p2.y);     

   if (x < 0 || y < 0) {
       return 0.f;
   }

   return sm->getMap(x,y, zDepth);

  }
};






























#endif
