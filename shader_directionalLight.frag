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
	float shine;
};

uniform Light directionalLight;
uniform Material material;

in vec3 normalDataOutput;
in vec3 vertexDataOutput;
in mat4 toWorldMatrix;

out vec4 outColor;

void main()
{

	
	mat3 normalMatrix = mat3(transpose(inverse(toWorldMatrix)));	//fix on toWorld matrix
	
	vec3 normal = normalize(normalMatrix * normalDataOutput);			//find normal vector in world coordinates

	vec3 fragPosition = vec3(toWorldMatrix * vec4(vertexDataOutput,1)); //normalize(normalMatrix * vertexDataOutput);		//find fragment position in world coordinates

	vec3 L = -1 * directionalLight.Direction;		//find surface to light vector L



	//Add diffuse componenet
	vec3 C_diffuse = material.diffuse * directionalLight.Color * max( dot(normal, L), 0.0f);


	//Add specular component
	vec3 eyePosition = vec3(0.0f, 0.0f, 20.0f);
	vec3 R = 2 * max(dot(normal, L),0.0f) * normal - L;
	vec3 e = normalize(eyePosition - fragPosition);
	vec3 C_specular = material.specular * directionalLight.Color * pow(dot(R, e), material.shine);

	//Add ambient component
	vec3 C_ambient = 0.2f * directionalLight.Color;

	outColor = vec4(C_diffuse + C_specular + C_ambient, 1) * vec4(material.materialColor,1);
	
}