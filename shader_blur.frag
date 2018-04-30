#version 330 core
#define PI 3.1415926535897932384626433832795

//this is the fragment shader to blur the current camera

uniform sampler2D texture;
uniform float blurRadius;
in vec3 pos;

out vec4 finalColor;
void main()
{
	vec2 pixelPosition = vec2( (pos.x / 2.0) + 0.5 , (pos.y /2.0) + 0.5);
	vec4 sumOfColors =	texture2D(texture, pixelPosition)  * 8;

	
	
	for(int iter = 0; iter < 16; ++iter){
		sumOfColors += texture2D(texture, pixelPosition + (blurRadius / 100.0) * vec2(cos(iter * (2 * PI) / 16), sin(iter * (2 * PI) / 16))) * 4;
	}
	for(int iter = 0; iter < 16; ++iter){
		sumOfColors += texture2D(texture, pixelPosition + 2 * (blurRadius / 100.0) * vec2(cos(iter * (2 * PI) / 16), sin(iter * (2 * PI) / 16))) * 2;
	}
	for(int iter = 0; iter < 16; ++iter){
		sumOfColors += texture2D(texture, pixelPosition + 3 * (blurRadius / 100.0) * vec2(cos(iter * (2 * PI) / 16), sin(iter * (2 * PI) / 16))) * 1;
	}

    finalColor =  sumOfColors / 120;

	

}
