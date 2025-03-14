#ifndef TEXTURES_H
#define TEXTURES_H
#include "stb_image.h"

#include <string>

class Texture {
    private:
        unsigned int texture_id;
        std::string file_path; // file path of the texture
        unsigned char* local_buffer; // local buffer for the texture
        int width, height, BPP; // width, height and bits per pixel (BPP)
    public:
        Texture(const std::string& path);
        // ~Texture();

        void bind(unsigned int slot = 0) const; // cosnt after - function is part of a class and cannot change any member of that class  
        void unbind() const;

};

#endif 