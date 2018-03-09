#version 330 core
// This is the skybox fragment shader.

// Note that you do not have access to the vertex shader's default output, gl_Position.

in vec3 objectSpacePosition;

layout (location = 0) out vec4 color;

uniform samplerCube skybox;
uniform vec3 camPosition;

void main()
{
	color = texture(skybox, objectSpacePosition);		//for skybox
	color.a = 1;
}
