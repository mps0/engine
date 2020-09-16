#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "vector.hpp"
#include "matrix.hpp"

//debug
#include<stdio.h>

class Camera{
    public:
        float sensitivity = 0.3f;
        float mouseSense = 0.01f;

        Vec3f up, forward, pos;
        Vec3f w, u, v;

        Camera(Vec3f up, Vec3f forward, Vec3f pos) : up(up), forward(forward), pos(pos) {
        
        update();
        
        }
       
    void roll(int xrel) {
		float rotationX = mouseSense * xrel;
    
        Vec3f upNew;
        upNew = cos(rotationX) * up - sin(rotationX) * u;

        up = upNew;
        
        update();
    }

	void rotate(int xrel, int yrel) {

		float rotationX = mouseSense * xrel;
		float rotationY = mouseSense * yrel;

        Vec3f forwardNew, upNew;
      //pitch
        forwardNew = cos(rotationY) * forward + sin(rotationY) * up; 
        upNew = cos(rotationY) * up - sin(rotationY) * forward;
        forward = forwardNew;
        up = upNew;

       //yaw
        forwardNew = cos(rotationX) * forward + sin(rotationX) * u;
        forward = forwardNew;


        update();
	}



        void update() {

         w = -forward.normalize();
         u = Vec3cross(up, w).normalize();
         v = Vec3cross(w, u);

        }
};

#endif
