#pragma once
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Node {
	
public:
	virtual void update() = 0;
	virtual void draw(glm::mat4 C) = 0;
};