#ifndef TEXTURE
#define TEXTURE
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>
#include <iostream>

class Material {

	glm::vec3 color;
	GLuint textureID;
	GLuint normalMapID;

	GLuint shaderProgram;

public:

	Material();
	~Material();
	void loadTexture(const char* filename);
	void loadNormalMap(const char* filename);
	void setShaderProgram(GLuint sp);
	void setColor(glm::vec3 c);
	GLuint getShaderProgram();
	GLuint getTextureID();
	GLuint getNormalMapID();
	glm::vec3 getColor();
	
private:
	void loadImage(const char* filename, GLuint &currID);
	unsigned char* loadPPM(const char* filename, int& width, int& height);

	
};

#endif