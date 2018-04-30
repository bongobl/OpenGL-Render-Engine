#pragma once
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

class Texture {

	unsigned int type;
	GLuint id;

public:
	Texture();
	void generatePlainTexture();
	void loadStandardTexture(const char* filename);
	void loadCubeMap(std::vector<std::string> faces);
	void disposeCurrentTexture();
	unsigned int getType();
	GLint getID();

	enum Types { INVALID, STANDARD, CUBE_MAP, PLAIN};
private:

	//Helpers
	void loadImage(const char* filename);
	void loadCubeMapTexture(std::vector<std::string> faces);
	unsigned char* loadPPM(const char* filename, int& width, int& height);

};