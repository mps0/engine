#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>


template<class T> class Vec2 {
    public:

        T x, y;

        Vec2() : x(0), y(0) {}
        Vec2(T x, T y) : x(x), y(y) {}

        void set(T x, T y) {
            this->x = x;
            this->y = y;
        }

        void swap() {
            T tmp = x;
            x = y;
            y = tmp;
        }

};
typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;

template<class T>
class Vec3 {
    public:
        T x, y, z;

        Vec3() : x(0), y(0), z(0){}
        Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

        float length() {
            return std::sqrt(x * x + y * y + z * z);
        }

        Vec3& operator-() {
            x = -x;
            y = -y;
            z = -z;
            return *this;
        }

        Vec3& normalize() {
            *this = (1.f / length()) * *this;
            return *this;
        }

};
typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;

template<class T>
Vec3<T> operator*(T a, Vec3<T> V) {
    return Vec3<T>(a * V.x, a * V.y, a * V.z);
}

template<class T>
Vec3<T> Vec3cross(Vec3<T> a, Vec3<T> b) {
    return Vec3<T>(a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x);
}

template<class T>
class Vec4 {
    public:
        T x, y, z, w;

        Vec4() : x(0), y(0), z(0), w(0){}
        Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}


};
typedef Vec4<int> Vec4i;
typedef Vec4<float> Vec4f;

template<class T>
T Vec4dot(Vec4<T> v0, Vec4<T> v1) {
    return v0.x * v1.x +
           v0.y * v1.y +
           v0.z * v1.z +
           v0.w * v1.w;
}



















#endif
