#version 330 core

//Vertex Attributes
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

//MVP matrices
uniform mat4 projection;
uniform mat4 view;
uniform mat4 toWorld;


//Output ports for vertex attributes
out vec3 objectSpacePosition;
out vec3 objectSpaceNormal;
out vec2 uvTexCoord;
out vec3 objectSpaceTangent;
out vec3 objectSpaceBitangent;

//output port for toWorld Matrix
out mat4 toWorldMatrix;


void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V and M
    gl_Position = projection * view * toWorld * vec4(position, 1.0);
	
	objectSpacePosition = position;
	objectSpaceNormal = normal;
	uvTexCoord = uv;
	objectSpaceTangent = tangent;
	objectSpaceBitangent = bitangent;

	toWorldMatrix = toWorld;
}
