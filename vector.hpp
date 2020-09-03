#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>


template<class T>
class vec2 {
    public:

        T x, y;

        vec2() : x(0), y(0) {}
        vec2(T x, T y) : x(x), y(y) {}

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
typedef vec2<int> vec2i;
typedef vec2<float> vec2f;

template<class T>
class vec3 {
    public:
        T x, y, z;

        vec3() : x(0), y(0), z(0){}
        vec3(T x, T y, T z) : x(x), y(y), z(z) {}

        float length() {
            return std::sqrt(x * x + y * y + z * z);
        }

        vec3& operator-() {
            x = -x;
            y = -y;
            z = -z;
            return *this;
        }

        vec3& normalize() {
            *this = (1.f / length()) * *this;
            return *this;
        }

};
typedef vec3<int> vec3i;
typedef vec3<float> vec3f;

vec3f operator*(float a, vec3f v) {
    return vec3f(a * v.x, a * v.y, a * v.z);
}

vec3f vec3cross(vec3f a, vec3f b) {
    return vec3f(a.y * b.z - a.z * b.y,
                 a.z * b.x - a.x * b.z,
                 a.x * b.y - a.y * b.x);
}

template<class T>
class vec4 {
    public:
        T x, y, z, w;

        vec4() : x(0), y(0), z(0), w(0){}
        vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

};
typedef vec4<int> vec4i;
typedef vec4<float> vec4f;



















#endif
