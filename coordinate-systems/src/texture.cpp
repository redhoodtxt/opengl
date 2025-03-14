#include "textures.h"
#include "stb_image.h"

#include <glad/glad.h>

#include <string>

Texture::Texture(const std::string& path){ // & - pass by reference
    texture_id = 0;
    file_path = path;
    local_buffer = nullptr;
    width = 0;
    height = 0;
    BPP = 0;

    stbi_set_flip_vertically_on_load(1); // flip the png because OpenGL reads from bottom left up, whereas png is vice versa
    local_buffer = stbi_load(path.c_str(), &width, &height, &BPP, 4); // 4 is rgba

    // generate and bind texture
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // specify filtering methods
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // set how to handle texture when the fall out of (0,0) - (1,1) range (aka texture wrapping)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // horizontal wrap, GL_REPEAT repeats the texture image.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // vertical wrap

    // specify a two-dimensional texture image
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer);
    
    // unbind texture 
    glBindTexture(GL_TEXTURE_2D, 0);

    // free the buffer
    if(local_buffer){
        stbi_image_free(local_buffer);
    }

}

// no need for a deconstructor that calls this function, it will access invalid memory after cleanup
// Texture::~Texture(){
//     glDeleteTextures(1, &texture_id);
// }

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot); // specify and activate texture slot 0 
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}