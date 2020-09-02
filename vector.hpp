#ifndef VECTOR_HPP
#define VECTOR_HPP



template<class T>
class vec2 {
    public:

        T x, y;
        
        vec2() : x(0), y(0) {}
        vec2(T x, T y) : x(x), y(y) {}



};


typedef vec2<int> vec2i;


















#endif
