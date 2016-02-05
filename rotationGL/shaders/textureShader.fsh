#version 410

// pass in our texture
uniform sampler2D sampler;  

// lighting variables
uniform vec3 direction; 
uniform vec3 ambient;
uniform vec3 intensity;
uniform vec3 diffuse;

// variables from vertex shaders
in vec2 f_tCoord;
in vec3 f_normal; 

// pass the calculated color out to the buffer
out vec4 fragColor;

void main ()
{
	vec3 norm = normalize(f_normal);
	// we're calculating the lighting color in case we want to turn that on
	float fDotProduct = max(0.0, dot(f_normal, direction));
	vec3 vDiffuseColor = diffuse * ambient * fDotProduct;

	// in the end I opted for non-diffuse uniform lighting, more like rotational
	fragColor = vec4(max(0.0,dot(-direction,f_normal)) * intensity, 1.0) * texture(sampler,f_tCoord);

	// create the red dot on the globe to represent the player character
	if (dot(norm,vec3(0,0,-0.2)) > 0.199)
		fragColor = vec4(1,0,0,0);
}
