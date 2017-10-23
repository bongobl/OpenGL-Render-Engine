#version 330 core
// this shader is used as a directional light

struct Light{
	vec3 Color;
	vec3 Direction;
};

struct Material{
	vec3 materialColor;
	float diffuse;
	float specular;
};

uniform Light directionalLight;
uniform Material material;

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
	color = vec4(   material.diffuse * dot(normal, L) * directionalLight.Color,  1.0f );


	//add specular component
	vec3 eyePosition = vec3(0.0f, 0.0f, 20.0f);
	vec3 R = 2 * dot(normal, L) * normal - L;
	vec3 fragPosition = normalize(normalMatrix * vertexDataOutput);
	vec3 e = eyePosition - fragPosition;
	color += vec4( material.specular * directionalLight.Color * dot(R, e),0 );

	//add ambient component
	
}