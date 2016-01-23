#version 410

uniform sampler2D sampler;  

/** Uniforms you will use for lighting (and maybe more...) **/ 
uniform vec3 direction; 
uniform vec3 ambient;
uniform vec3 intensity;
uniform vec3 diffuse;

in vec2 f_tCoord;
in vec3 f_normal; 

out vec4 fragColor;

void main ()
{
    vec3 norm = normalize(f_normal); //renormalize the normnal 
	
	float fDotProduct = max(0.0, dot(f_normal, direction));
	vec3 vDiffuseColor = diffuse * ambient * fDotProduct;

	fragColor = vec4(/*vDiffuseColor +*/ max(0.0,dot(-direction,norm)) * intensity, 1.0) * texture(sampler,f_tCoord);
}
