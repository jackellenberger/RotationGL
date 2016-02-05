#version 410

// variables from cpu
uniform mat4 modelView;
uniform mat4 projection;
uniform vec3 translation;
uniform sampler2D sampler;  

// variables from mesh vao's
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tCoord;

// variables to send to fragment shader
out vec2 f_tCoord;
out vec3 f_normal; 

void main ()
{
	// calculate the vertex positions
	gl_Position = projection * modelView * vec4(translation + position,1.0);
	// pass along the texture coordinates
	f_tCoord = tCoord;
	// calculate normal vectors
	f_normal = normalize((projection * modelView * vec4(normal,0.0)).xyz) ;
}