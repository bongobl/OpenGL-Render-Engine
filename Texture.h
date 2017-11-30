#ifndef TEXTURE
#define TEXTURE
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>
#include <iostream>

class Texture {

	GLuint textureID;

public:

	Texture();

	void loadTexture(const char* filename);
	GLuint getTextureID();

private:
	unsigned char* loadPPM(const char* filename, int& width, int& height);

	
};

#endif