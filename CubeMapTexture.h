#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
using namespace std;

class CubeMapTexture {

public:

	GLuint textureID;

	void loadCubeMapTexture(vector<std::string> faces);
	GLuint getTextureID();

private:
	unsigned char* loadPPM(const char* filename, int& width, int& height);
};