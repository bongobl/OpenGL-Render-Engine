#include "Scene.h"
#include "shader.h"
#include <iostream>

void Scene::initObjects() {

	//set up UBO info for all scene lights
	for (unsigned int i = 0; i < 30; ++i) {

			allSceneLightStructs.push_back(LightStruct());
	}

	glUseProgram(Material::getShaderProgram());
	GLuint SceneLightsLocation = glGetUniformBlockIndex(Material::getShaderProgram(), "SceneLights");
	glUniformBlockBinding(Material::getShaderProgram(), SceneLightsLocation, 0);
	
	glGenBuffers(1, &UBO_Lights);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_Lights);
	glBufferData(GL_UNIFORM_BUFFER, 30 * sizeof(LightStruct), NULL, GL_DYNAMIC_DRAW);
	
	glBindBufferBase(GL_UNIFORM_BUFFER, SceneLightsLocation, UBO_Lights);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);


	//Set up Shadow Map (Not used yet)
	glGenFramebuffers(1, & shadowFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBuffer);

	shadowDepthTexture.generatePlainTexture();
	glBindTexture(GL_TEXTURE_2D, shadowDepthTexture.getID());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowDepthTexture.getID(), 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		exit(1);
	}

	initThisScenesObjects();
}

void Scene::dispose() {

	disposeThisScenesObjects();

	shadowDepthTexture.disposeCurrentTexture();
	glDeleteBuffers(1, &shadowFrameBuffer);
	glDeleteBuffers(1, &UBO_Lights);
}

void Scene::update(float deltaTime) {
	updateThisScenesObjects(deltaTime);

	for (unsigned int i = 0; i < allSceneCameras.size(); ++i) {

		allSceneCameras.at(i)->updateViewMatrix();
	}
}

void Scene::draw() {

	applyAllLights();
	drawThisScenesObjects();

}

void Scene::resize_event(int width, int height) {
	window_width = width;
	window_height = height;


	for (unsigned int i = 0; i < allSceneCameras.size(); ++i) {
		
		if (allSceneCameras.at(i) != NULL) 
			allSceneCameras.at(i)->resize((float)window_width, (float)window_height);
	}
}

//PRIVATE HELPERS

void Scene::applyAllLights() {

	//Material shader program is the only one that uses light calculations
	glUseProgram(Material::getShaderProgram());
	

	glUniform1i(glGetUniformLocation(Material::getShaderProgram(), "numLights"), allSceneLights.size());
	for (unsigned int i = 0; i < allSceneLights.size(); ++i) {
		allSceneLightStructs[i] = allSceneLights[i]->getLightStruct();
	}

	
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_Lights);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 30 * sizeof(LightStruct), allSceneLightStructs.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
}


