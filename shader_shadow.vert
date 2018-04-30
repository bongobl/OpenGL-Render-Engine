#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 position;

uniform mat4 lightProjection;
uniform mat4 lightView;
uniform mat4 toWorld;

void main(){
	gl_Position = lightProjection * lightView * toWorld * vec4(position, 1.0);
}