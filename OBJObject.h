#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#define GLFW_INCLUDE_GLEXT

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
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
private:

	//Model Geometry Data
	std::vector<GLuint> indices;
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;

	//Raw Object Properties	
	glm::vec3 position;
	float scale;
	float modelAngle;
	float modelZoom;

	//Transformation Matrices to reflect object properties 
	glm::mat4 toWorld;
	glm::mat4 translateMatrix;
	glm::mat4 scaleMatrix;

	//Rendering with modern OpenGL
	GLuint VBO, VBO2, VAO, EBO;
	GLuint uProjection, uModelview;
	GLuint uToWorld;

	//object's shader program
	GLuint shaderProgram;
	
public:


	OBJObject(const char* filepath, GLuint sp);
	~OBJObject();
	void parse(const char* filepath);
	void setDefaultProperties();


	//OpenGL draw function
	void draw();

	void update();

	//Manipulate raw Object Properties
	void setPosition(glm::vec3 newPosition);
	void move(glm::vec3 displacement);
	void setScale(float scaleVal);
	void incrementScale(float scaleDiff);


private:

};

#endif