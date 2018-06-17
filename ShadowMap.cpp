#include <iostream>
#include "ShadowMap.h"
#include "shader.h"
using namespace std;

GLuint ShadowMap::shaderProgram = -1;

//matrix to put coords range 0-1 to sample shadow map
glm::mat4 ShadowMap::biasMatrix;

//manage statics
void ShadowMap::initStatics() {
	shaderProgram = LoadShaders("../shader_shadow.vert", "../shader_shadow.frag");
	biasMatrix = glm::mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
}
void ShadowMap::cleanUpStatics() {
	glDeleteProgram(shaderProgram);
	shaderProgram = -1;
}

GLuint ShadowMap::getShaderProgram() {
	return shaderProgram;
}

ShadowMap::ShadowMap() {
	frameBuffer = -1;
	resolution = 4096;
	projectionMatrix = glm::ortho<float>(-100, 100, -100, 100, -200, 200);
}
ShadowMap::~ShadowMap() {
	disposeBufferAndTexture();
}
void ShadowMap::initBufferAndTexture() {

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	depthTexture.generatePlainTexture();
	glBindTexture(GL_TEXTURE_2D, depthTexture.getID());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//attach texture to frame buffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture.getID(), 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cerr << "Frame buffer's status reported incomplete\n" << endl;
		exit(1);
	}
}

void ShadowMap::disposeBufferAndTexture() {

	depthTexture.disposeCurrentTexture();
	if (frameBuffer != -1) {
		glDeleteBuffers(1, &frameBuffer);
		frameBuffer = -1;
	}
}
void ShadowMap::applyAttributes(Light* curr_light) {

	//set display matrix for shadowmap texture
	glViewport(0, 0, resolution, resolution);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//send matrices to shadow shader
	glm::vec3 lightDir = curr_light->getToWorld() * glm::vec4(0, 0, 1, 0);	//4th component 0 to avoid translations 
	glm::mat4 lightViewMatrix = glm::lookAt(glm::vec3(0, 0, 0), lightDir, glm::vec3(0, 1, 0));
	curr_light->setViewProjectionMatrix(biasMatrix * projectionMatrix * lightViewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "lightProjection"), 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "lightView"), 1, GL_FALSE, &lightViewMatrix[0][0]);
}


Texture ShadowMap::getDepthTexture() {
	return depthTexture;
}
