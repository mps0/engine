#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vector.hpp"

//template<class T, class U, class V, class W>
template<class T>
class Mat4 {
    public:


Vec4<T> c0, c1, c2, c3;

Mat4() : c0(Vec4<T>()), c1(Vec4<T>()), c2(Vec4<T>()), c3(Vec4<T>()) {}

Mat4(Vec4<T> c0, Vec4<T> c1, Vec4<T> c2, Vec4<T> c3) : c0(c0), c1(c1), c2(c2), c3(c3) {}


Mat4 operator*(Mat4 m) {
    Mat4 result;
    Mat4 t = transpose();

    result.c0 = Vec4<T>(Vec4dot(t.c0, m.c0), 
                        Vec4dot(t.c1, m.c0), 
                        Vec4dot(t.c2, m.c0), 
                        Vec4dot(t.c3, m.c0));

    result.c1 = Vec4<T>(Vec4dot(t.c0, m.c1), 
                        Vec4dot(t.c1, m.c1), 
                        Vec4dot(t.c2, m.c1), 
                        Vec4dot(t.c3, m.c1));

    result.c2 = Vec4<T>(Vec4dot(t.c0, m.c2), 
                        Vec4dot(t.c1, m.c2), 
                        Vec4dot(t.c2, m.c2), 
                        Vec4dot(t.c3, m.c2));

    result.c3 = Vec4<T>(Vec4dot(t.c0, m.c3), 
                        Vec4dot(t.c1, m.c3), 
                        Vec4dot(t.c2, m.c3), 
                        Vec4dot(t.c3, m.c3));

   return result;
}


Mat4 transpose() {
    Mat4 result;

    result.c0 = Vec4<T>(c0.x, c1.x, c2.x, c3.x);
    result.c1 = Vec4<T>(c0.y, c1.y, c2.y, c3.y);
    result.c2 = Vec4<T>(c0.z, c1.z, c2.z, c3.z);
    result.c3 = Vec4<T>(c0.w, c1.w, c2.w, c3.w);

    return result;
}

Vec4<T> operator*(Vec4<T> v) {
    Vec4<T> result;
    Mat4 t = transpose();

    result.x = Vec4dot(t.c0, v);
    result.y = Vec4dot(t.c1, v);
    result.z = Vec4dot(t.c2, v);
    result.w = Vec4dot(t.c3, v);

    return result;
}

void print() {
    
            printf("(%f, %f, %f, %f)\n", c0.x, c1.x, c2.x, c3.x);
            printf("(%f, %f, %f, %f)\n", c0.y, c1.y, c2.y, c3.y);
            printf("(%f, %f, %f, %f)\n", c0.z, c1.z, c2.z, c3.z);
            printf("(%f, %f, %f, %f)\n", c0.w, c1.w, c2.w, c3.w);

}



};
typedef Mat4<float> Mat4f;


template<class T>
class Mat3 {
    public:


Vec3<T> c0, c1, c2;

Mat3() : c0(Vec3<T>()), c1(Vec3<T>()), c2(Vec3<T>()) {}

Mat3(Vec3<T> c0, Vec3<T> c1, Vec3<T> c2) : c0(c0), c1(c1), c2(c2) {}


Mat3 operator*(Mat3 m) {
    Mat3 result;
    Mat3 t = transpose();

    result.c0 = Vec3<T>(Vec3dot(t.c0, m.c0), 
                        Vec3dot(t.c1, m.c0), 
                        Vec3dot(t.c2, m.c0));

    result.c1 = Vec3<T>(Vec3dot(t.c0, m.c1), 
                        Vec3dot(t.c1, m.c1), 
                        Vec3dot(t.c2, m.c1));

    result.c2 = Vec3<T>(Vec3dot(t.c0, m.c2), 
                        Vec3dot(t.c1, m.c2), 
                        Vec3dot(t.c2, m.c2));


   return result;
}


Mat3 transpose() {
    Mat3 result;

    result.c0 = Vec3<T>(c0.x, c1.x, c2.x);
    result.c1 = Vec3<T>(c0.y, c1.y, c2.y);
    result.c2 = Vec3<T>(c0.z, c1.z, c2.z);

    return result;
}

Vec3<T> operator*(Vec3<T> v) {
    Vec3<T> result;
    Mat3 t = transpose();

    result.x = Vec3dot(t.c0, v);
    result.y = Vec3dot(t.c1, v);
    result.z = Vec3dot(t.c2, v);

    return result;
}
};
typedef Mat3<float> Mat3f;






































#endif
