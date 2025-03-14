#version 330 core
out vec4 frag_texture;
in vec2 tex_coord;
uniform sampler2D ourTexture; // give access to texture object

void main()
{
    frag_texture = texture(ourTexture, tex_coord);  
};
