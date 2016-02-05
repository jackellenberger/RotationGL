#version 410

// pass in our texture
uniform sampler2D sampler;

// pass in variables from the cpu
uniform vec4 color;	//the rgba value of the color of this box
uniform vec3 openFace;	//the face of the box that is open

// grab variables from vertex shader
in vec2 f_tCoord;
in vec3 f_normal; 

out vec4 fragColor;

void main ()
{
	// just throw out vertices that are on the open edge of the box
   	if (f_normal == openFace)
   		discard;

   	// calculate the alpha value depending on the texture
	fragColor = texture(sampler,f_tCoord).aaaa;

	// if this is a colored pixel, set the color to the passed in color
	if (fragColor.r > 0){
		fragColor.xyz = color.rgb;
	}
	
	// lighten the four sides that border the open face
	if (f_normal != (openFace * vec3(-1))){
		fragColor.xyz = fragColor.xyz + vec3(0.7,0.7,0.7);
	}
}