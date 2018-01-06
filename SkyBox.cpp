#include "SkyBox.h"
#include "Scene.h"

SkyBox::SkyBox()
{
	setLocalScale(glm::vec3(1000,1000,1000));
	shaderProgram = LoadShaders("../shader.vert", "../shader_skybox.frag");
	
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	

	glBindVertexArray(VAO);

	//Vertex Position
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.


	// We've sent the vertex data over to OpenGL, but there's still something missing.
	// In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	//Unbind current VBO and VAO, but not EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
}

SkyBox::~SkyBox()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteProgram(shaderProgram);
}

void SkyBox::draw(Scene* currScene) {

	glUseProgram(shaderProgram);

	Camera* activeCamera = currScene->getActiveCamera();
	
	//apply skybox properties
	this->applySettings();

	//apply camera properties
	activeCamera->applySettings(shaderProgram);

	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);

	glDepthMask(GL_FALSE);

	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glDepthMask(GL_TRUE);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);

	drawAllChildren(currScene);
}

void SkyBox::loadCubeMapTexture(Texture cube_map_texture) {

	if(cube_map_texture.getType() == Texture::CUBE_MAP)
		this->cubeMapTexture = cube_map_texture;
	else {
		std::cerr << "ERROR: texture for skybox must be a of type cubemap" << std::endl;
	}
}

void SkyBox::applySettings() {

	//send toWorld to shader
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "toWorld"), 1, GL_FALSE, &toWorld[0][0]);

	//send cubemap textureID to shader
	glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture.getID());
}