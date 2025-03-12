#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec2 aTextCoord;

// out vec4 vertex_colour;
out vec3 vertex_colour;
out vec2 tex_coord;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
// 	vertex_colour = vec4(0.5, 0.0, 0.0, 1.0);
    vertex_colour = aColour;
    tex_coord = aTextCoord;
};