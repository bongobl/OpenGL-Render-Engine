#pragma once
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

struct Texture {

	enum Types {NONE, STANDARD, CUBE_MAP};

	unsigned int type;
	GLuint id;

	Texture();
	void loadStandardTexture(const char* filename);
	void loadCubeMap(std::vector<std::string> faces);
	void disposeCurrentTexture();

	GLint getID();

private:
	//Helpers
	void loadImage(const char* filename);
	void loadCubeMapTexture(std::vector<std::string> faces);
	unsigned char* loadPPM(const char* filename, int& width, int& height);

};