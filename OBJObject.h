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

class OBJObject
{
private:

	//Object Geometry Data
	std::vector<GLuint> indices;
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;

	glm::mat4 toWorld;
	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;


	//Raw Object Properties
	glm::vec3 objectCenterOffset;
	glm::vec3 position;
	float modelAngle;
	float size;
	float orientation;	

	//Transformation Matrices to adhere to object properties 
	glm::mat4 translateMatrix;
	glm::mat4 spinMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 orbitMatrix;

	//Point Size to draw
	GLfloat pointSize;
	

public:
	OBJObject(const char* filepath);
	~OBJObject();
	void parse(const char* filepath);

	//OpenGL draw function
	void draw(GLuint shaderProgram);


	void update();


	//Manipulate raw Object Properties
	void setPosition(glm::vec3 newPosition);
	void setSize(float newSize);
	void setOrientation(float newOrientation);

	void spin(float rad);
	void move(glm::vec3 displacement);
	void incrementSize(float offset);
	void orbit(float orbitVal);
	void incrementPointSize(GLfloat offset);


};

#endif