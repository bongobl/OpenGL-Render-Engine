#version 330 core
// this shader is used as a point light

struct Light{
	vec3 Color;
	vec3 Position;
};

struct Material{
	vec3 materialColor;
	float diffuse;
	float specular;
	float shine;
};

uniform Light pointLight;
uniform Material material;

in vec3 normalDataOutput;
in vec3 vertexDataOutput;
in mat4 toWorldMatrix;

out vec4 outColor;

void main(){

	
	mat3 normalMatrix = transpose(inverse(mat3(toWorldMatrix)));		//fix on toWorld matrix
	
	vec3 normal = normalize(normalMatrix * normalDataOutput);			//find normal vector in world coordinates
	
	vec3 fragPosition = vec3(toWorldMatrix * vec4(vertexDataOutput,1));		//find fragment position in world coordinates
	
	vec3 L = normalize(pointLight.Position - fragPosition);						//find surface to light vector L 

	vec3 C_l = pointLight.Color / length(pointLight.Position - fragPosition);	//find intensity at fragment


	//Add diffuse componenet
	vec3 C_diffuse = material.diffuse * C_l * max( dot(normal, L), 0.0f);

	//Add specular component
	vec3 eyePosition = vec3(0.0f, 0.0f, 20.0f);
	vec3 R = 2 * dot(normal, L) * normal - L;
	vec3 e = normalize(eyePosition - fragPosition);
	vec3 C_specular = material.specular * C_l * pow( max(dot(R, e),0) , material.shine);

	//Add ambient component
	vec3 C_ambient = 0.1f * C_l;

	outColor = vec4(C_diffuse + C_specular + C_ambient, 1) * vec4(material.materialColor,1);

}