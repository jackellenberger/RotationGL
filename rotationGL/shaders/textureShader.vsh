#version 410

uniform mat4 modelView;
uniform mat4 projection;
uniform vec3 translation;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tCoord;

out vec2 f_tCoord;
out vec3 f_normal; 

void main ()
{
   gl_Position = projection * modelView * vec4(translation + position,1.0);
   f_tCoord = tCoord;
   f_normal = (projection * modelView * vec4(normal,0.0)).xyz ;
   
}