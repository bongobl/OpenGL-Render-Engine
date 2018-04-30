#pragma once

#define GLFW_INCLUDE_GLEXT
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "Material.h"
#include "ShadowMap.h"
#include "SceneObject.h"
class Scene;

class Model : public SceneObject
{

	//Model Geometry Data
	std::vector<GLuint> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;

	//centers model geometry
	glm::vec3 meshCenterOffset;
	glm::mat4 centerModelMeshMatrix;

	//Rendering with modern OpenGL
	GLuint VBO_positions, VBO_normals, VBO_uvs, VBO_tangents, VBO_bitangents, VAO, EBO;

	//object's material
	Material material;
	
public:

	Model(const char* filepath, Material m);
	~Model();
	void parse(const char* filepath);

	

	//override
	void sendThisGeometryToShadowMap();
	void drawThisSceneObject(Scene* currScene);

	void setMaterial(Material m);
	Material& getMaterial();
	void centerMesh(bool opt);
	glm::mat4 getToWorldWithCenteredMesh();
	std::vector<glm::vec3> getVertices();

private:
	void applySettings();
};