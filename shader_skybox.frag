#version 330 core
// This is the skybox fragment shader.

// Note that you do not have access to the vertex shader's default output, gl_Position.

in vec3 objectSpacePosition;

out vec4 color;

uniform samplerCube skybox;


void main()
{
	color = texture(skybox, objectSpacePosition);		//for skybox
	
}
