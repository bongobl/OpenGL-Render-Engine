#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#define GLFW_INCLUDE_GLEXT
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "Material.h"

class OBJObject
{

	//Model Geometry Data
	std::vector<GLuint> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;

	//centers model geometry
	glm::mat4 centerModelMesh;

	//model center
	glm::vec3 modelCenter;


	//Transformation Matrices to reflect object properties 
	glm::mat4 toWorld;

	//Rendering with modern OpenGL
	GLuint VBO_positions, VBO_normals, VBO_uvs, VBO_tangents, VBO_bitangents, VAO, EBO;
	GLuint uProjection, uModelview;
	GLuint uToWorld;

	//object's material
	Material material;
	
public:

	OBJObject(const char* filepath, Material m);
	~OBJObject();
	void parse(const char* filepath);

	//to manipulate toWorld
	void setToWorld(glm::mat4 M_new);

	void setModelCenter(glm::vec3 newCenter);

	void setMaterial(Material m);

	std::vector<glm::vec3> getVertices();

	friend class Scene;
};

#endif