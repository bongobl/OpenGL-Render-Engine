#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Material.h"

class Scene;
class BoundingBox {
	
	//fields
	glm::vec3 lowest;
	glm::vec3 highest;
	std::vector<glm::vec3> meshVertices;
	std::vector<glm::vec3> boxVertices;


	//drawing
	Material material;
	GLuint VAO, VBO;


public:
	BoundingBox(std::vector<glm::vec3> verts);
	~BoundingBox();

	bool isCollidingWith(const BoundingBox* other);
	void updateToWorld(glm::mat4 toWorld);
	void draw(Scene* currScene);
};