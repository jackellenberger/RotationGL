#version 410

uniform sampler2D sampler;  
uniform vec4 color;
uniform vec3 edge;


in vec2 f_tCoord;
in vec3 f_normal; 

out vec4 fragColor;

void main ()
{
   	if (f_normal == edge)
   		discard;
    vec3 norm = normalize(f_normal); //renormalize the normnal 
	vec4 col = color;
	fragColor = texture(sampler,f_tCoord).aaaa;
	if (fragColor.r > 0){
		fragColor.xyz = color.rgb;
	}
	if (f_normal != (edge * vec3(-1))){
		fragColor.xyz = fragColor.xyz + vec3(0.7,0.7,0.7);
	}
}