#version 330 core
// This is the material fragment shader.

struct Light{
	int type; 			 vec3 typePadding;
	vec3 color;			 float colorPadding;
	float brightness;	 vec3 brightnessPadding;
	vec3 position;		 float positionPadding;
	vec3 direction;		 float directionPadding;
	
};

struct Material{

	//DIFFUSE
	int useDiffuse;
	vec3 diffuse;

	//SPECULAR
	int useSpecular;
	vec3 specular;

	//AMBIENT
	int useAmbient;
	vec3 ambient;

	//SURFACE COLOR
	int useSurfaceColor;
	vec3 surfaceColor;

	//SURFACE TEXTURE
	int useSurfaceTexture;
	sampler2D surfaceTexture;
	float surfaceTextureStrength;

	//NORMAL MAP
	int useNormalMap;
	sampler2D normalMap;
	float normalMapStrength;

	//REFLECTION TEXTURE
	int useReflectionTexture;
	samplerCube reflectionTexture;
	float reflectiveness;
	
};

uniform Material material;
uniform Light light;
uniform vec3 camPosition;

in vec3 objectSpacePosition;
in vec3 objectSpaceNormal;
in vec2 uvTexCoord;
in vec3 objectSpaceTangent;
in vec3 objectSpaceBitangent;

in mat4 toWorldMatrix;

layout (location = 0) out vec4 outColor;


//constants
const int DIRECTIONAL_LIGHT = 0;
const int POINT_LIGHT = 1;

//current light properties
vec3 L = vec3(0,0,0);
float C_l = 0;
void main()
{

	//FIND POSITION, NORMAL AND TANGENTS IN WORLD COORDINATES
	vec3 world_position = vec3(toWorldMatrix * vec4(objectSpacePosition,1));

	mat3 refinedToWorld = mat3(transpose(inverse(toWorldMatrix)));	
	vec3 world_normal = normalize(refinedToWorld * objectSpaceNormal);				
	vec3 world_tangent = normalize(refinedToWorld * objectSpaceTangent);
	vec3 world_bitangent = normalize(refinedToWorld * objectSpaceBitangent);

	
	//find values L and C_l based on light properties
	
	if(light.type == DIRECTIONAL_LIGHT){
		L = -normalize(light.direction);
		C_l = light.brightness;
	}else if(light.type == POINT_LIGHT){
		
		L = normalize(light.position - world_position);
		C_l = light.brightness / length(light.position - world_position);

	}

	//Starting Color: white, each material property will cut away at it
	outColor = vec4(1,1,1,1);

	//define variables out here for ambient lighting to use
	vec4 textureColor = vec4(1,1,1,1);
	vec4 reflectionTextureColor = vec4(1,1,1,1);

	//Textures
	if(material.useSurfaceColor == 1){
		outColor *= vec4(material.surfaceColor,1);
	}
	if(material.useSurfaceTexture == 1){
		textureColor = (1 -  material.surfaceTextureStrength * (1 - texture2D(material.surfaceTexture, uvTexCoord)));
		outColor *= textureColor;
	}
	
	if(material.useNormalMap == 1){

		vec3 normalOffset = normalize(texture2D( material.normalMap, uvTexCoord ).rgb * 2.0 - 1.0);
		world_normal = normalize(world_normal + normalOffset * material.normalMapStrength);

	}	
	if(material.useReflectionTexture == 1){
		vec3 I = normalize(world_position - camPosition);
		vec3 R = reflect(I, normalize(world_normal));
		reflectionTextureColor =  1 - (material.reflectiveness * (1 - vec4(texture(material.reflectionTexture, R).rgb, 1.0)));
		outColor *= reflectionTextureColor;
	}
	
	
	vec4 multiplier = vec4(0,0,0,1);

	//Lighting Modes
	if(material.useDiffuse == 1){
		multiplier += vec4(material.diffuse,0) * max( dot(world_normal, L), 0) * vec4(light.color * C_l ,1);
		outColor *= multiplier;
	}
	if(material.useSpecular == 1){
		vec3 R = 2 * dot(world_normal, L) * world_normal - L;	//reflect(-L, world_normal);
		vec3 e = normalize(camPosition - world_position);
		outColor += vec4(material.specular,0) * pow( max(dot(R, e),0) , 20) * vec4(light.color * C_l ,1);
	}		
	
	if(material.useAmbient == 1){
		outColor += vec4(material.ambient, 0) * textureColor * reflectionTextureColor * vec4(light.color * C_l ,1);
	}

	
}

