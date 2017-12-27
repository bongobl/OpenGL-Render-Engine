#ifndef MATERIAL
#define MATERIAL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
class Material {

	//static fields
	static GLuint shaderProgram;
	static std::vector<GLuint> allIDs;

	//member fields
	glm::vec3 color;
	GLuint textureID;
	GLuint normalMapID;
	GLuint reflectionTextureID;

	
public:
	bool useColor;
	bool useTexture;
	bool useNormalMap;
	bool useReflectionTexture;

	bool useLighting;

	static void initStatics();
	static void cleanUpStatics();
	static Material basic();
	Material();
	~Material();

	void setUseLighting(bool opt);
	void setColor(glm::vec3 c);
	void loadTexture(const char* filename);
	void loadNormalMap(const char* filename);
	void loadReflectionTexture(std::vector<std::string> faces);
	GLuint getTextureID();
	GLuint getNormalMapID();
	glm::vec3 getColor();
	
	static GLuint getShaderProgram();

	void applySettings();

//Helper
	static void loadImage(const char* filename, GLuint &currID);
	static void loadCubeMapTexture(std::vector<std::string> faces, GLuint &currID);
	static unsigned char* loadPPM(const char* filename, int& width, int& height);
	
};

#endif