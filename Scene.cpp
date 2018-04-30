#include "Scene.h"
#include "shader.h"
#include <iostream>


void Scene::init() {

	//set up UBO info for all scene lights
	for (unsigned int i = 0; i < MAX_LIGHTS; ++i) {

			allSceneLightStructs.push_back(LightStruct());
	}
	
	glGenBuffers(1, &UBO_Lights);	//will be deleted in first resize

	light0ShadowMap.initBufferAndTexture();

	initThisScene();
}

void Scene::dispose() {

	disposeThisScene();

	light0ShadowMap.disposeBufferAndTexture();
	glDeleteBuffers(1, &UBO_Lights);
}

void Scene::update() {
	updateThisScene();

	for (unsigned int i = 0; i < allSceneCameras.size(); ++i) {

		//for cameras in target mode, need to keep View and
		//toWorld matrices up to date with target
		allSceneCameras.at(i)->updateViewMatrix();
	}
}

/*update shadow map objects for lights, this is done before drawing*/
void Scene::calcShadowMaps() {


	glUseProgram(ShadowMap::getShaderProgram());	//set active shader

	light0ShadowMap.applyAttributes(allSceneLights[0]);	//also sets Light's VP matrix in biased form

	drawThisSceneToShadowMap();	//draw scene to shadow map
	
}
void Scene::draw() {

	//apply shadow map to material shader
	glUseProgram(Material::getShaderProgram());	

	//send shadow maps to material shader, start at 3, since 0-2 used by material textures
	glUniform1i(glGetUniformLocation(Material::getShaderProgram(), "shadowMaps[0]"), 3);
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, light0ShadowMap.getDepthTexture().getID());

	applyAllLights();

	drawThisScene();

}

void Scene::resize_event(int width, int height) {

	window_width = width;
	window_height = height;

	for (unsigned int i = 0; i < allSceneCameras.size(); ++i) {
		
		if (allSceneCameras.at(i) != NULL) 
			allSceneCameras.at(i)->resize((float)window_width, (float)window_height);
	}

	recalcUBO_Lights();
}


//PRIVATE HELPERS
void Scene::applyAllLights() {

	//Material shader program is the only one that uses light calculations
	glUseProgram(Material::getShaderProgram());

	//update light structs to reflect scene light attributes
	glUniform1i(glGetUniformLocation(Material::getShaderProgram(), "numLights"), allSceneLights.size());
	for (unsigned int i = 0; i < allSceneLights.size(); ++i) {
		allSceneLightStructs[i] = allSceneLights[i]->getLightStruct();
	}

	//send buffer over to material shader
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_Lights);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, MAX_LIGHTS * sizeof(LightStruct), allSceneLightStructs.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
}

void Scene::recalcUBO_Lights() {

	glUseProgram(Material::getShaderProgram());
	GLuint SceneLightsLocation = glGetUniformBlockIndex(Material::getShaderProgram(), "SceneLights");
	glUniformBlockBinding(Material::getShaderProgram(), SceneLightsLocation, 0);

	glDeleteBuffers(1, &UBO_Lights);
	glGenBuffers(1, &UBO_Lights);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_Lights);
	glBufferData(GL_UNIFORM_BUFFER, MAX_LIGHTS * sizeof(LightStruct), NULL, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, SceneLightsLocation, UBO_Lights);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

