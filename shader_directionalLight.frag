#version 330 core
// this shader is used as a directional light

struct Light{
	vec3 Color;
	vec3 Direction;
};
uniform Light directionalLight;

in vec3 normalDataOutput;
in vec3 vertexDataOutput;
in mat4 toWorldMatrix;

out vec4 color;

void main()
{

	//find normal vector in world coordinates
	mat3 normalMatrix = transpose(inverse(mat3(toWorldMatrix)));
	vec3 normal = normalize(normalMatrix * normalDataOutput);

	//find surface to light vector L
	vec3 L = -1 * directionalLight.Direction;

	//add diffuse componenet
	color = vec4( dot(normal, L) * directionalLight.Color,  1.0f );
	//color = vec4(normalDataOutput, 1);
	
}