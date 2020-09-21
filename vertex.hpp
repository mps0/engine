#ifndef VERTEX_HPP
#define VERTEX_HPP



class Vertex{
    public:


        Vec4f pos;
        Vec3f normal;
        Vec2f uv;
        Vec4f worldPos;
        bool visible = true;

        Vertex() : pos(Vec4f(0.f, 0.f, 0.f, 1.f)), normal(Vec3f(0.f, 1.f, 0.f)), uv(Vec2f(0.f, 0.f)) {}

        Vertex(Vec4f pos, Vec4f color, Vec3f normal, Vec2f uv): pos(pos), normal(normal), uv(uv) {}


};

























#endif
