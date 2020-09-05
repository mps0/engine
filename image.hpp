#ifndef IMAGE_HPP
#define IMAGE_HPP


class Image{
    public:
    const unsigned short width, height;
    unsigned int* pixels;

        Image() : width(0), height(0) {}
        Image(const unsigned short width, const unsigned short height) :width(width), height(height) {
      
         pixels = new unsigned int[width * height];
         //default white background
         memset(pixels, 255, width * height * sizeof(unsigned int));
        }

        ~Image(){
        delete[] pixels;
        }

        void setPixel(unsigned int x, unsigned int y, unsigned int val){
            
           unsigned int idx = y * width + x; 
           pixels[idx] = val;
        }

        void clear() {

        
         memset(pixels, 255, width * height * sizeof(unsigned int));

        }

};















#endif
