#version 410

uniform mat4 modelView;
uniform mat4 projection;
uniform vec3 translation;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 f_normal; 

void main ()
{
   gl_Position = projection * modelView * vec4(translation + position,1.0);
   f_normal = normal;
   
}