#ifndef TEXTURE
#define TEXTURE
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>
#include <iostream>

class Material {

	//static fields
	static GLuint shaderProgram;

	//member fields
	glm::vec3 color;
	GLuint textureID;
	GLuint normalMapID;

	
public:
	bool useColor;
	bool useTexture;
	bool useNormalMap;
	bool useLighting;

	static void initStatics();
	static void cleanUpStatics();
	Material();
	~Material();

	void setUseLighting(bool opt);
	void setColor(glm::vec3 c);
	void loadTexture(const char* filename);
	void loadNormalMap(const char* filename);
		
	GLuint getTextureID();
	GLuint getNormalMapID();
	glm::vec3 getColor();
	
	GLuint getShaderProgram();

	void applySettings();
private:
	void loadImage(const char* filename, GLuint &currID);
	unsigned char* loadPPM(const char* filename, int& width, int& height);
	
};

#endif