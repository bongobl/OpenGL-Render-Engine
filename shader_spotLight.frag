#version 330 core
// this shader is used as a spot light

struct Light{
	vec3 Color;
	vec3 Position;
	vec3 Direction;
	//float spot_cutoff;
	//float spot_exponent;
};

struct Material{
	vec3 materialColor;
	float diffuse;
	float specular;
};

uniform Light spotLight;
uniform Material material;

in vec3 normalDataOutput;
in vec3 vertexDataOutput;
in mat4 toWorldMatrix;

out vec4 outColor;

void main(){

	//temp constants
	float spot_cutoff = 0.8f;
	float spot_exponent = 1;

	mat3 normalMatrix = transpose(inverse(mat3(toWorldMatrix)));		//fix on toWorld matrix
	
	vec3 normal = normalize(normalMatrix * normalDataOutput);			//find normal vector in world coordinates
	
	vec3 fragPosition = normalize(normalMatrix * vertexDataOutput);		//find fragment position in world coordinates
	
	vec3 L = normalize(spotLight.Position - fragPosition);						//find surface to light vector L 

	vec3 C_l = spotLight.Color / length(spotLight.Position - fragPosition);		//find intensity at fragment


	if( dot(-1 * L, spotLight.Direction) <= cos(spot_cutoff)){
		C_l = vec3(0,0,0);
	}else{
		C_l = spotLight.Color * pow(dot(-1 * L, spotLight.Direction), spot_exponent) / pow(length(spotLight.Position - fragPosition),2);
	}

	//C_l = spotLight.Color * pow(dot(-1 * L, spotLight.Direction), spot_exponent) / pow(length(spotLight.Position - fragPosition),2);


	//Add diffuse componenet
	vec3 C_diffuse = material.diffuse * C_l * max( dot(normal, L), 0.0f);

	//Add specular component
	vec3 eyePosition = vec3(0.0f, 0.0f, 20.0f);
	vec3 R = 2 * max(dot(normal, L),0.0f) * normal - L;
	vec3 e = normalize(eyePosition - fragPosition);
	vec3 C_specular = material.specular * C_l * pow(dot(R, e), 32);

	//Add ambient component
	vec3 C_ambient = vec3(0.2f,0.2f,0.2f);

	outColor = vec4(C_diffuse + C_specular + C_ambient, 1); //vec4(material.materialColor,1);

}