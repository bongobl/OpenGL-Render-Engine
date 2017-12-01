#ifndef OBJOBJECT_H
#define OBJOBJECT_H


#define GLFW_INCLUDE_GLEXT

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
//#include <OpenGL/gl.h> // Remove this line in future projects
#else

#include <GL/glew.h>

#endif


#include <GLFW/glfw3.h>

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "Material.h"

class OBJObject
{

	//Model Geometry Data
	std::vector<GLuint> indices;
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;

	//centers model geometry
	glm::mat4 centerModelMesh;

	//model center
	glm::vec3 modelCenter;


	//Transformation Matrices to reflect object properties 
	glm::mat4 toWorld;

	//Rendering with modern OpenGL
	GLuint VBO, VBO2, VAO, EBO;
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

	//OpenGL draw function
	void draw();

	void setModelCenter(glm::vec3 newCenter);

	void setMaterial(Material m);
};

#endif