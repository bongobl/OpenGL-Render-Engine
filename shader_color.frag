#version 330 core
// This is color fragment shader.

struct Material{
	vec3 color;
	sampler2D image;
	sampler2D normalMap;
};

uniform Material material;

in vec3 normalDataOutput;
in vec3 vertexDataOutput;
in mat4 toWorldMatrix;
in vec3 TexCoords;

out vec4 outColor;

void main()
{
	mat3 normalMatrix = mat3(transpose(inverse(toWorldMatrix)));	//fix on toWorld matrix
	
	vec3 normal = normalize(normalMatrix * normalDataOutput);			//find normal vector in world coordinates

	vec3 fragPosition = vec3(toWorldMatrix * vec4(vertexDataOutput,1));		//find fragment position in world coordinates		//find fragment position in world coordinates

	vec3 L = vec3(1,1,1);		//find surface to light vector L

	outColor = vec4(material.color,1) * max( dot(normal, L), 0.0f);	
}
