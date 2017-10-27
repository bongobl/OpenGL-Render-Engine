#version 330 core
// this shader is used as a spot light

struct Light{
	vec3 Color;
	vec3 Position;
	vec3 Direction;
	float spot_cutoff;
	float spot_exponent;
};

struct Material{
	vec3 materialColor;
	float diffuse;
	float specular;
	float shine;
};

uniform Light spotLight;
uniform Material material;

in vec3 normalDataOutput;
in vec3 vertexDataOutput;
in mat4 toWorldMatrix;

out vec4 outColor;

void main(){



	mat3 normalMatrix = transpose(inverse(mat3(toWorldMatrix)));		//fix on toWorld matrix
	
	vec3 normal = normalize(normalMatrix * normalDataOutput);			//find normal vector in world coordinates
	
	vec3 fragPosition = vec3(toWorldMatrix * vec4(vertexDataOutput,1));		//find fragment position in world coordinates
	
	vec3 L = normalize(spotLight.Position - fragPosition);						//find surface to light vector L 

	vec3 C_l = spotLight.Color / length(spotLight.Position - fragPosition);		//find intensity at fragment


	if( dot(-1 * L, spotLight.Direction) <= cos(spotLight.spot_cutoff)){
		C_l = vec3(0,0,0);
	}else{
		C_l = spotLight.Color * pow(dot(-1 * L, spotLight.Direction), spotLight.spot_exponent) / pow(length(spotLight.Position - fragPosition),2);
	}


	//Add diffuse componenet
	vec3 C_diffuse = material.diffuse * C_l * max( dot(normal, L), 0.0f);

	//Add specular component
	vec3 eyePosition = vec3(0.0f, 0.0f, 20.0f);
	vec3 R = 2 * dot(normal, L) * normal - L;
	vec3 e = normalize(eyePosition - fragPosition);
	vec3 C_specular = material.specular * C_l * pow( max(dot(R, e),0) , material.shine);

	//Add ambient component
	vec3 C_ambient = 0.1f * spotLight.Color;

	outColor = vec4(C_diffuse + C_specular + C_ambient, 1) * vec4(material.materialColor,1);

}