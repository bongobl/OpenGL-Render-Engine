#version 330 core
// This is the material fragment shader.

struct DirectionalLight{
	vec3 direction;
	vec3 color;
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
};

uniform Material material;
uniform DirectionalLight directionalLight;
uniform vec3 camPosition;

in vec3 vertexDataOutput;
in vec3 normalDataOutput;
in vec2 uvOutput;
in vec3 tangentOutput;
in vec3 bitangentOutput;

in mat4 toWorldMatrix;

out vec4 outColor;

void main()
{

	//FIND POSITION, NORMAL AND TANGENTS IN WORLD COORDINATES
	vec3 world_position = vec3(toWorldMatrix * vec4(vertexDataOutput,1));

	mat3 normalMatrix = mat3(transpose(inverse(toWorldMatrix)));	
	vec3 world_normal = normalize(normalMatrix * normalDataOutput);			
	
	vec3 world_tangent = vec3(toWorldMatrix * vec4(tangentOutput,1));
	vec3 world_bitangent = vec3(toWorldMatrix * vec4(bitangentOutput,1));

	//LIGHT DIRECTION
	vec3 L = normalize(directionalLight.direction);		

	

	mat3 tangent2World = mat3(
		world_tangent,
		world_bitangent,
		world_normal
	);
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
		reflectionTextureColor = vec4(texture(material.reflectionTexture, R).rgb, 1.0);
		outColor *= reflectionTextureColor;
	}
	
	if(material.useDiffuse)
		outColor *= material.diffuse * vec4(directionalLight.color,1) * max( dot(world_normal, L), 0);
		
	if(material.useSpecular){
		vec3 R = 2 * dot(world_normal, L) * world_normal - L;
		vec3 e = normalize(camPosition - world_position);
		outColor += material.specular * directionalLight.color * pow( max(dot(R, e),0) , 20);
	}		
	if(material.useAmbient){
		outColor += vec4(material.ambient, 0) * textureColor * reflectionTextureColor;
	}
		
}
