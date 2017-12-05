#version 330 core
// This is a texture and normal fragment shader.

struct Material{
	bool useColor;
	bool useTexture;
	bool useNormalMap;
	bool useLighting;

	vec3 color;
	sampler2D texture;
	sampler2D normalMap;
};

uniform Material material;


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
	vec3 L = normalize(vec3(1,-0.5f,1));		

	outColor = vec4(1,1,1,1);

	mat3 tangent2World = mat3(
		world_tangent,
		world_bitangent,
		world_normal
	);
	
	if(material.useColor)
		outColor *= material.color;
	if(material.useTexture)
		outColor *= texture2D(material.texture, uvOutput);
	if(material.useNormalMap){
		vec3 normalOffset = normalize(texture2D( material.normalMap, uvOutput ).rgb*2.0 - 1.0);
		world_normal = normalize(world_normal + normalOffset);
	}
	if(material.useLighting)
		outColor *= max( dot(world_normal, L), 0.0f);	

}
