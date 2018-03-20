#include "Scene.h"
#include "shader.h"
#include <iostream>

void Scene::initObjects() {

	initThisScenesObjects();

	
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
	
	
}

void Scene::dispose() {
	disposeThisScenesObjects();

	glDeleteBuffers(1, &UBO_Lights);
}

void Scene::resize_event(int width, int height) {
	window_width = width;
	window_height = height;


	for (unsigned int i = 0; i < allSceneCameras.size(); ++i) {
		
		if (allSceneCameras.at(i) != NULL) 
			allSceneCameras.at(i)->resize((float)window_width, (float)window_height);
	}
}

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

