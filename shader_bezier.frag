#version 330 core
// This is the point fragment shader.

// Note that you do not have access to the vertex shader's default output, gl_Position.

in vec3 normalDataOutput;
in vec3 vertexDataOutput;

in vec3 TexCoords;
out vec4 color;


uniform vec3 inColor;

void main()
{
	color = vec4(0,0,0, 1);	//black color
}
