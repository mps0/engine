#ifndef VECTOR_HPP
#define VECTOR_HPP



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
            this->x = y;
            this->y = tmp;
        }
        




};


typedef vec2<int> vec2i;


















#endif
