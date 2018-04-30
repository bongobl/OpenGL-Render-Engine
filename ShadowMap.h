#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.h"
#include "Light.h"


class ShadowMap {

	static GLuint shaderProgram;
	static glm::mat4 biasMatrix;

	GLuint frameBuffer;
	Texture depthTexture;

	GLuint resolution;
	glm::mat4 projectionMatrix;
	
public:

	//manage statics
	static void initStatics();
	static void cleanUpStatics();
	static GLuint getShaderProgram();


	ShadowMap();
	~ShadowMap();
	void initBufferAndTexture();
	void disposeBufferAndTexture();
	void applyAttributes(Light* curr_light);

	Texture getDepthTexture();

};