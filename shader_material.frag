#version 330 core
// This is the material fragment shader.

struct Light{
	int type;
	vec3 color;
	float brightness;
	vec3 position;
	vec3 direction;
	
};

struct Material{
	bool useDiffuse;
	bool useSpecular;
	bool useAmbient;
	bool useTexture;
	bool useNormalMap;
	bool useReflectionTexture;

	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
	sampler2D texture;
	sampler2D normalMap;
	samplerCube reflectionTexture;
	float reflectiveness;
};

uniform Material material;
uniform Light light;
uniform vec3 camPosition;

in vec3 vertexDataOutput;
in vec3 normalDataOutput;
in vec2 uvOutput;
in vec3 tangentOutput;
in vec3 bitangentOutput;

in mat4 toWorldMatrix;

out vec4 outColor;


//constants
const int DIRECTIONAL_LIGHT = 0;
const int POINT_LIGHT = 1;
void main()
{

	//FIND POSITION, NORMAL AND TANGENTS IN WORLD COORDINATES
	vec3 world_position = vec3(toWorldMatrix * vec4(vertexDataOutput,1));

	mat3 normalMatrix = mat3(transpose(inverse(toWorldMatrix)));	
	vec3 world_normal = normalize(normalMatrix * normalDataOutput);			
	
	vec3 world_tangent = vec3(toWorldMatrix * vec4(tangentOutput,1));
	vec3 world_bitangent = vec3(toWorldMatrix * vec4(bitangentOutput,1));

	vec3 L = vec3(1,1,1);
	float C_l = 1;
	if(light.type == DIRECTIONAL_LIGHT){
		L = -normalize(light.direction);
		C_l = light.brightness;
	}else if(light.type == POINT_LIGHT){
		
		L = normalize(light.position - world_position);
		C_l = light.brightness / length(light.position - world_position);


	}

	//Starting Color: white, each material property will cut away at it
	outColor = vec4(1,1,1,1);

	vec4 textureColor = vec4(1,1,1,1);
	vec4 reflectionTextureColor = vec4(1,1,1,1);

	if(material.useTexture){
		textureColor = texture2D(material.texture, uvOutput);
		outColor *= textureColor;
	}
	if(material.useNormalMap){
		vec3 normalOffset = normalize(texture2D( material.normalMap, uvOutput ).rgb*2.0 - 1.0);
		world_normal = normalize(world_normal + normalOffset);
	}	
	if(material.useReflectionTexture){
		vec3 I = normalize(world_position - camPosition);
		vec3 R = reflect(I, normalize(world_normal));
		reflectionTextureColor =  1 - (material.reflectiveness * (1 - vec4(texture(material.reflectionTexture, R).rgb, 1.0)));
		outColor *= reflectionTextureColor;
	}
	
	if(material.useDiffuse)
		outColor *= vec4(material.diffuse,1) * max( dot(world_normal, L), 0);
		
	if(material.useSpecular){
		vec3 R = 2 * dot(world_normal, L) * world_normal - L;
		vec3 e = normalize(camPosition - world_position);
		outColor += material.specular * pow( max(dot(R, e),0) , 20);
	}		
	outColor *= vec4(light.color * C_l ,1);

	if(material.useAmbient){
		outColor += vec4(material.ambient, 0) * textureColor * reflectionTextureColor;
	}
	
		
}
