#ifndef ASSETS_HPP
#define ASSETS_HPP

#include "vector.hpp"
#include "pipeline.hpp"
#include "vertex.hpp"


class Asset{
    public:
        Vertex*   vBuffer;
        int*    iBuffer;
        int num_vertices;
        int num_triangles;

        Asset() : vBuffer(nullptr), iBuffer(nullptr) {}

        Asset(int num_vertices, int num_triangles) : num_vertices(num_vertices), num_triangles(num_triangles)  {

            vBuffer = new Vertex[num_vertices];
            iBuffer = new int[num_triangles];

        }
        ~Asset() {
            delete[] vBuffer;
            delete[] iBuffer; 
        }
};


class Triangle : public Asset{

    public:
    Vec3f p0, p1, p2;
    Vec4f c0, c1, c2;


    Triangle(Vec3f p0, Vec3f p1, Vec3f p2, Vec4f c0, Vec4f c1, Vec4f c2) : p0(p0), p1(p1), p2(p2), c0(c0), c1(c1), c2(c2){

       num_vertices = 3;
       num_triangles = 1;
       vBuffer = new Vertex[num_vertices]; 
       iBuffer = new int[num_triangles];

       vBuffer[0].pos = Vec4f(p0.x, p0.y, p0.z, 1.f);
       vBuffer[1].pos = Vec4f(p1.x, p1.y, p1.z, 1.f);
       vBuffer[2].pos = Vec4f(p2.x, p2.y, p2.z, 1.f);

       vBuffer[0].color = c0;
       vBuffer[1].color = c1;
       vBuffer[2].color = c2;

       iBuffer[0] = 0;
       iBuffer[1] = 1;
       iBuffer[2] = 2;



    }



};


class Box : public Asset {
    public:    

        Vec3f origin;
        float width, length, height;

        Box(Vec3f origin, float width, float length, float height) :
            origin(origin), width(width), length(length), height(height) {

                num_triangles = 12;
                num_vertices = 8;
                Vec4f color = Vec4f(1.f, 0.f, 0.f, 1.f);
                Vec4f p0 = Vec4f(-0.5f * width + origin.x, -0.5f * length + origin.y, -0.5f * height + origin.z, 1.f); 
                Vec4f p1 = Vec4f(0.5f * width + origin.x, -0.5f * length + origin.y, -0.5f * height + origin.z, 1.f); 
                Vec4f p2 = Vec4f(-0.5f * width + origin.x, 0.5f * length + origin.y, -0.5f * height + origin.z, 1.f); 
                Vec4f p3 = Vec4f(0.5f * width + origin.x, 0.5f * length + origin.y, -0.5f * height + origin.z, 1.f); 

                Vec4f p4 = p0 + Vec4f(0.f, 0.f, height, 0.f);
                Vec4f p5 = p1 + Vec4f(0.f, 0.f, height, 0.f);
                Vec4f p6 = p2 + Vec4f(0.f, 0.f, height, 0.f);
                Vec4f p7 = p3 + Vec4f(0.f, 0.f, height, 0.f);

                vBuffer = new Vertex[num_vertices];
                vBuffer[0].pos = p0;
                vBuffer[0].pos = p0;
                vBuffer[1].pos = p1;
                vBuffer[2].pos = p2;
                vBuffer[3].pos = p3;

                vBuffer[4].pos = p4;
                vBuffer[5].pos = p5;
                vBuffer[6].pos = p6;
                vBuffer[7].pos = p7;


                vBuffer[0].color = color; 
                vBuffer[0].color = color; 
                vBuffer[1].color = color; 
                vBuffer[2].color = color; 
                vBuffer[3].color = color; 
                                        ;
                vBuffer[4].color = color; 
                vBuffer[5].color = color; 
                vBuffer[6].color = color; 
                vBuffer[7].color = color; 


                iBuffer = new int[num_triangles * 3];
                //bottom face
                iBuffer[0] = 0;
                iBuffer[1] = 2;
                iBuffer[2] = 3;

                iBuffer[3] = 3;
                iBuffer[4] = 1;
                iBuffer[5] = 0;


                //left face
                iBuffer[12] = 2;
                iBuffer[13] = 6;
                iBuffer[14] = 4;

                iBuffer[15] = 4;
                iBuffer[16] = 0;
                iBuffer[17] = 2;

                //right face
                iBuffer[18] = 1;
                iBuffer[19] = 5;
                iBuffer[20] = 7;

                iBuffer[21] = 7;
                iBuffer[22] = 3;
                iBuffer[23] = 1;

                //back face
                iBuffer[24] = 2;
                iBuffer[25] = 6;
                iBuffer[26] = 7;

                iBuffer[27] = 7;
                iBuffer[28] = 3;
                iBuffer[29] = 2;

                //top face
                iBuffer[30] = 4;
                iBuffer[31] = 6;
                iBuffer[32] = 7;

                iBuffer[33] = 7;
                iBuffer[34] = 5;
                iBuffer[35] = 4;

                //front face
                iBuffer[6] = 0;
                iBuffer[7] = 4;
                iBuffer[8] = 5;

                iBuffer[9] = 5;
                iBuffer[10] = 1;
                iBuffer[11] = 0;

            }
};



class Grid : public Asset {
    public:
        Vec3f origin;
        float width, length;
        int rows, cols;


        Grid (Vec3f origin, float width, float length, int rows, int cols) : 
            origin(origin), width(width), length(length), rows(rows), cols(cols) {

                num_vertices = (rows + 1) * (cols + 1);
                num_triangles = rows * cols * 2;

                Vec4f color = Vec4f(1.f, 1.f, 1.f, 1.f);

                vBuffer = new Vertex[num_vertices];
                iBuffer = new int[num_triangles * 3];



                float xstep = width / cols;
                float ystep = length / rows;

                float xstart = origin.x - 0.5f * width;
                float ystart = origin.y + 0.5f * length;

                for (int i = 0; i < rows + 1; i++) {
                    for (int j = 0; j < cols + 1; j++) {

                        vBuffer[i * (cols + 1) + j].pos = Vec4f(j * xstep + xstart, ystart - i* ystep, origin.z, 1.f);
                        vBuffer[i * (cols + 1) + j].color = color;
                    }
                }

                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        int startIdx = 6 * (i * (cols) + j);

                        iBuffer[startIdx] = (i + 1) * (cols + 1) + j;
                        iBuffer[startIdx + 1] = i * (cols + 1) + j;
                        iBuffer[startIdx + 2] = i * (cols + 1) + j + 1;
                        iBuffer[startIdx + 3] = i * (cols + 1) + j + 1;;
                        iBuffer[startIdx + 4] = (i + 1) * (cols + 1) + j + 1;
                        iBuffer[startIdx + 5] = (i + 1) * (cols + 1) + j;

                    }
                }
            }

};




















#endif
