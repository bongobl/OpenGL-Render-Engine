#include "Scene.h"

void Scene::initObjects() {

	cam_pos = glm::vec3(0.0f, 0, 30);
	cam_look_at = glm::vec3(0.0f, 0.0f, 0.0f);
	cam_up = glm::vec3(0.0f, 1.0f, 0.0f);

	asteroid = new Asteroid(0);
	asteroid->setRotationSpeed(3);

	//vector of skybox face names
	std::vector<std::string> faceNames;
	faceNames.push_back("skybox/left.ppm");
	faceNames.push_back("skybox/right.ppm");
	faceNames.push_back("skybox/top.ppm");
	faceNames.push_back("skybox/bottom.ppm");
	faceNames.push_back("skybox/front.ppm");
	faceNames.push_back("skybox/back.ppm");

	//init cubemap
	spaceView.loadCubeMapTexture(faceNames);
}
void Scene::dispose() {
	delete asteroid;
}

void Scene::update(float deltaTime) {
	if (glfwGetKey(SceneManager::window, GLFW_KEY_LEFT)) {
		asteroid->setRotationSpeed(-3);
	}else if (glfwGetKey(SceneManager::window, GLFW_KEY_RIGHT)) {
		asteroid->setRotationSpeed(3);
	}
	else {
		asteroid->setRotationSpeed(0);
	}
	asteroid->update(deltaTime);
}
void Scene::draw() {

	drawCubeMap(spaceView);
	asteroid->draw();
	drawOBJObject(asteroid->asteroidOBJ[0]);
}

void Scene::key_event(int key, int scancode, int action, int mods) {
	
}
void Scene::resize_event(int width, int height) {

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}
void Scene::mouse_button_event(int button, int action, int mods) {

}
void Scene::cursor_position_event(double xpos, double ypos) {

}
void Scene::mouse_wheel_event(double xoffset, double yoffset) {

}

void Scene::drawOBJObject(OBJObject &obj) {
	
	glUseProgram(obj.material.getShaderProgram());

	glm::mat4 modelCenterMatrix = glm::translate(glm::mat4(1.0f), obj.modelCenter);
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = V * obj.toWorld * modelCenterMatrix;// *centerModelMesh;
																	  // We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
																	  // Consequently, we need to forward the projection, view, and model matrices to the shader programs
																	  // Get the location of the uniform variables "projection" and "modelview"
	GLuint uProjection = glGetUniformLocation(obj.material.getShaderProgram(), "projection");
	GLuint uModelview = glGetUniformLocation(obj.material.getShaderProgram(), "modelview");
	GLuint uToWorld = glGetUniformLocation(obj.material.getShaderProgram(), "toWorld");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(uToWorld, 1, GL_FALSE, &obj.toWorld[0][0]);


	// Now draw this OBJObject. We simply need to bind the VAO associated with it.
	glBindVertexArray(obj.VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.EBO);


	//material properties
	obj.material.applySettings();


	glDrawElements(GL_TRIANGLES, obj.indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Scene::drawCubeMap(CubeMap &cubeMap) {
	
	glUseProgram(cubeMap.shaderProgram);

	cubeMap.toWorld = glm::translate(glm::mat4(1.0f), cubeMap.position) * glm::scale(glm::mat4(1.0f), glm::vec3(cubeMap.scale, cubeMap.scale, cubeMap.scale));

	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = V * cubeMap.toWorld;
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	cubeMap.uProjection = glGetUniformLocation(cubeMap.shaderProgram, "projection");
	cubeMap.uModelview = glGetUniformLocation(cubeMap.shaderProgram, "modelview");
	// Now send these values to the shader program
	glUniformMatrix4fv(cubeMap.uProjection, 1, GL_FALSE, &P[0][0]);
	glUniformMatrix4fv(cubeMap.uModelview, 1, GL_FALSE, &modelview[0][0]);
	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(cubeMap.VAO);


	glDepthMask(GL_FALSE);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.textureID);

	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glDepthMask(GL_TRUE);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}