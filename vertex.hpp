#ifndef VERTEX_HPP
#define VERTEX_HPP



class Vertex{
    public:


        Vec4f pos, color;

        Vertex() : pos(Vec4f(0.f, 0.f, 0.f, 1.f)), color(Vec4f(0.f, 0.f, 0.f, 1.f)) {}
        Vertex(Vec4f pos, Vec4f color) : pos(pos), color(color) {}


};

























#endif
