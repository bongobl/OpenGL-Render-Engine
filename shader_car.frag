#version 330 core
// This is the car fragment shader.

// Note that you do not have access to the vertex shader's default output, gl_Position.

in vec3 normalDataOutput;
in vec3 vertexDataOutput;

in vec3 TexCoords;
out vec4 color;

uniform vec3 cameraPos;

//how does frag shader get this?
uniform samplerCube skybox;

uniform vec3 inColor;

void main()
{

	vec3 I = normalize(vertexDataOutput - cameraPos);
    vec3 R = reflect(I, normalize(normalDataOutput));
    color = vec4(texture(skybox, R).rgb, 1.0);
	
}