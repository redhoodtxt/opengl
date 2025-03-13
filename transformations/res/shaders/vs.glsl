#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTextCoord;

out vec2 tex_coord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0f); // multiply the position vector with the transformation matrix

    tex_coord = vec2(aTextCoord.x, aTextCoord.y); // for texture 
    
};