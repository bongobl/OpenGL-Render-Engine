#version 330 core
// this shader is used as a directional light

in vec3 normalDataOutput;
in vec3 vertexDataOutput;
in mat4 toWorldMatrix;

out vec4 color;

void main()
{
	//hard coded light properties
	float C_src = .83f;
	vec3 lightDirection = normalize(vec3(-1,-1,-1));

	//find normal vector in world coordinates
	mat3 normalMatrix = transpose(inverse(mat3(toWorldMatrix)));
	vec3 normal = normalize(normalMatrix * normalDataOutput);

	//find surface to light vector L
	vec3 L = -1 * lightDirection;

	//add diffuse componenet
	//color = vec4( C_src * dot(normal, L) * vec3(1,1,1),  1.0f );
	color = vec4(normalDataOutput, 1);
	
}