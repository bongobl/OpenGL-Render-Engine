#version 330 core
// This is the material fragment shader.


#define DIRECTIONAL_LIGHT	0
#define POINT_LIGHT			1
#define MAX_LIGHTS			30

//Light struct definition
struct Light{
		 
	vec4 color;					
	vec4 position;		 
	vec4 direction;	
	
	int type; 
	float brightness;
	float padding;
	float padding2;

	mat4 VP;
};

//Material struct definition
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

layout (std140) uniform SceneLights {
	Light allLights[MAX_LIGHTS];
};

uniform sampler2D shadowMaps[MAX_LIGHTS];


uniform Material material;
uniform vec3 camPosition;
uniform int numLights;

//from vertex shader
in vec3 objectSpacePosition;
in vec3 objectSpaceNormal;
in vec2 uvTexCoord;
in vec3 objectSpaceTangent;
in vec3 objectSpaceBitangent;
in mat4 toWorldMatrix;

layout (location = 0) out vec4 outColor;


//current light properties
vec3 L = vec3(0,0,0);
float C_l = 0;

//model properties
vec3 world_position;
mat3 refinedToWorld;
vec3 world_normal;			
vec3 world_tangent;
vec3 world_bitangent;


//prototype
void calc_LandC_L(int i);


void main()
{
	
	//FIND POSITION, NORMAL AND TANGENTS IN WORLD COORDINATES
	world_position = vec3(toWorldMatrix * vec4(objectSpacePosition,1));

	refinedToWorld = mat3(transpose(inverse(toWorldMatrix)));	
	world_normal = normalize(refinedToWorld * objectSpaceNormal);				
	world_tangent = normalize(refinedToWorld * objectSpaceTangent);
	world_bitangent = normalize(refinedToWorld * objectSpaceBitangent);
	

	//Starting Color: white, each material property will cut away at it
	outColor = vec4(1,1,1,1);

	//define variables out here for ambient lighting to use
	vec4 surfaceTextureColor = vec4(1,1,1,1);
	vec4 reflectionTextureColor = vec4(1,1,1,1);

	//Textures
	if(material.useSurfaceColor == 1){
		outColor *= vec4(material.surfaceColor,1);
	}
	if(material.useSurfaceTexture == 1){
		surfaceTextureColor = (1 -  material.surfaceTextureStrength * (1 - texture2D(material.surfaceTexture, uvTexCoord)));
		outColor *= surfaceTextureColor;
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
	float visibility = 1.0f;

	//Lighting Modes
	if(material.useDiffuse == 1){

		int currShadowMap = 0;
		for(int i = 0; i < numLights; ++i){
			calc_LandC_L(i);
			
			visibility = 1.0f;
			if(allLights[i].type == DIRECTIONAL_LIGHT){
				vec4 lightSpaceposition = allLights[i].VP * vec4(world_position,1);
				if (texture(shadowMaps[currShadowMap], lightSpaceposition.xy ).z  <  lightSpaceposition.z - 0.005){
					visibility = 0;
				}
				++currShadowMap;
			}

			multiplier += visibility * vec4(material.diffuse,0) * max( dot(world_normal, L), 0) * allLights[i].color * C_l;
		}

		outColor *= multiplier;
	}
	if(material.useSpecular == 1){
		for(int i = 0; i < numLights; ++i){
			calc_LandC_L(i);
			vec3 R = 2 * dot(world_normal, L) * world_normal - L;	//reflect(-L, world_normal);
			vec3 e = normalize(camPosition - world_position);
			outColor += vec4(material.specular,0) * pow( max(dot(R, e),0) , 20) * allLights[i].color * C_l;
		}
	}		
	
	if(material.useAmbient == 1){
		for(int i = 0; i < numLights; ++i){
			calc_LandC_L(i);
			outColor += vec4(material.ambient, 0) * surfaceTextureColor * reflectionTextureColor * allLights[i].color * C_l;
		}
	}

	
}//END MAIN


void calc_LandC_L(int i){

	if(allLights[i].type == DIRECTIONAL_LIGHT){

		L = -normalize(allLights[i].direction.xyz);
		C_l = allLights[i].brightness;
	}else if(allLights[i].type == POINT_LIGHT){

		L = normalize(allLights[i].position.xyz - world_position);
		C_l = allLights[i].brightness / length(allLights[i].position.xyz - world_position);

	}
}