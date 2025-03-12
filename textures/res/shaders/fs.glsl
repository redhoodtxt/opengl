#version 330 core
out vec4 frag_colour;
// in vec4 vertex_colour; // takes the output of vertex shader as input to the fragment shader
in vec3 vertex_colour; // takes the output of vertex shader as input to the fragment shader
// uniform vec4 new_colour; // using uniform keyword - another way of passing data into shaders
in vec2 tex_coord;
uniform sampler2D ourTexture; // give access to texture object

void main()
{
//    frag_colour = vertex_colour;
//   	frag_colour = new_colour;
    frag_colour = texture(ourTexture, tex_coord) /** vec4(vertex_colour, 1.0)*/;
};
