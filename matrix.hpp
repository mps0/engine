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

};
typedef Mat4<float> Mat4f;








































#endif
