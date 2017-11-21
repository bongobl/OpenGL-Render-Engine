#version 330 core


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 normalDataOutput;
out vec3 vertexDataOutput;
out mat4 toWorldMatrix;

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 toWorld;




void main()
{

	normalDataOutput = mat3(transpose(inverse(toWorld))) * normal;
    vertexDataOutput = vec3(toWorld * vec4(position, 1.0));
	toWorldMatrix = toWorld;
    gl_Position = projection * modelview * vec4(position, 1.0);

}
