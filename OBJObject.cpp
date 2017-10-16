//NOTE SOMETHING WROTE WITH std::vectors

//#define GLEW_GET_FUN
#include <iostream>
#include <string>
#include <cmath>
#include "OBJObject.h"
#include "Window.h"
using namespace std;



OBJObject::OBJObject(const char *filepath) 
{


	//read in geometry data disk
	parse(filepath);
	/*
	//Find Object Center
	float lowestX, highestX, lowestY, highestY, lowestZ, highestZ;
	lowestX = highestX = vertices[0].x;
	lowestY = highestY = vertices[0].y;
	lowestZ = highestZ = vertices[0].z;

	for (int i = 0; i < vertices.size(); ++i) {
		if (vertices[i].x > highestX)	highestX = vertices[i].x;
		if (vertices[i].x < lowestX)	lowestX =  vertices[i].x;
		if (vertices[i].y > highestY)	highestY = vertices[i].y;
		if (vertices[i].y < lowestY)	lowestY = vertices[i].y;
		if (vertices[i].z > highestZ)	highestZ = vertices[i].z;
		if (vertices[i].z < lowestZ)	lowestZ = vertices[i].z;
	}
	objectCenterOffset.x = (highestX + lowestX) / 2.0f;
	objectCenterOffset.y = (highestY + lowestY) / 2.0f;
	objectCenterOffset.z = (highestZ + lowestZ) / 2.0f;
	

	//initialize Object Properties
	this->position = glm::vec3(0, 0, 0);
	this->size = 1.0f;
	this->modelAngle = 0.0f;
	this->orientation = 0;
	
	//Initialize transformation matrices to reflect object properties.
	translateMatrix = glm::translate(glm::mat4(1.0f), position);
	spinMatrix = glm::rotate(glm::mat4(1.0f), modelAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(size, size, size));
	orbitMatrix = glm::rotate(glm::mat4(1.0f), this->orientation, glm::vec3(0.0f, 0.0f, 1.0f));
	
	//initlaize point size
	this->pointSize = 1;
	*/

	
	toWorld = glm::mat4(1.0f);
	
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);	
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

					 // We've sent the vertex data over to OpenGL, but there's still something missing.
					 // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), indices.data(), GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
	
}

OBJObject::~OBJObject() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
void OBJObject::parse(const char *filepath) 
{

	// Populate the face indices, vertices, and normals vectors with the OBJ Object data	
	FILE* fp;
	char currLine[BUFSIZ];
	GLfloat x, y, z;
	GLfloat r, g, b;
	GLuint v1, v2, v3, n1, n2, n3;

	fp = fopen(filepath, "rb");
	if (fp == NULL) {
		cerr << "error loading file" << endl;
		exit(-1);
	}
	cout << "Parsing " << string(filepath) << "..."<< endl;

	while (fgets(currLine, BUFSIZ, fp) != NULL) {

		//Process vertex or vertex normal
		if (currLine[0] == 'v') {
			if (currLine[1] == ' ') {
				sscanf(currLine + 2, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
				//cout << x << " " << y << " " << z << endl;
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
			}
			else if (currLine[1] == 'n' && currLine[2] == ' ') {
				sscanf(currLine + 3, "%f %f %f", &x, &y, &z);
				normals.push_back(x);
				normals.push_back(y);
				normals.push_back(z);
			}
		}
		//process face
		else if (currLine[0] == 'f' && currLine[1] == ' ') {		
			sscanf(currLine + 2, "%i//%i %i//%i %i//%i", &v1, &n1, &v2, &n2, &v3, &n3);
			indices.push_back(v1);
			indices.push_back(n1);
			indices.push_back(v2);
			indices.push_back(n2);
			indices.push_back(v3);
			indices.push_back(n3);
		}
		
	}
	cout << "num vertices: " << vertices.size() << endl;
	cout << "num normals: " << normals.size() << endl;
	cout << "num indices: " << indices.size() << endl;

	

}

void OBJObject::draw(GLuint shaderProgram)
{
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}





void OBJObject::setPosition(glm::vec3 newPosition) {
	position = newPosition;
	translateMatrix = glm::translate(glm::mat4(1.0f), position);
}
void OBJObject::setSize(float newSize) {
	size = newSize;
	scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(size, size, size));
}
void OBJObject::setOrientation(float newOrientation) {
	orientation = newOrientation;
	orbitMatrix = glm::rotate(glm::mat4(1.0f), this->orientation / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
}

void OBJObject::update() {

	spin(0.2f * glm::pi<float>() / 180.0f);
}

void OBJObject::spin(float rad) {

	this->modelAngle += rad;
	if (this->modelAngle > 360.0f || this->modelAngle < -360.0f) this->modelAngle = 0.0f;

	// This creates the matrix to rotate the object
	this->spinMatrix = glm::rotate(glm::mat4(1.0f), this->modelAngle, glm::vec3(0.0f, 1.0f, 0.0f));

	this->toWorld = glm::rotate(glm::mat4(1.0f), this->modelAngle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void OBJObject::move(glm::vec3 displacement) {
	position = position + displacement;
	translateMatrix = glm::translate(glm::mat4(1.0f), position);
}

void OBJObject::incrementSize(float offset) {
	size += offset;
	scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(size, size, size));
}

void OBJObject::orbit(float orbitVal) {

	orientation += orbitVal;

	float objectAngleXY = atan2(position.y, position.x);
	float objectDistanceXY = sqrt(position.x * position.x + position.y * position.y);

	position.x = objectDistanceXY * cos(objectAngleXY + orbitVal);
	position.y = objectDistanceXY * sin(objectAngleXY + orbitVal);

	orbitMatrix = glm::rotate(glm::mat4(1.0f), this->orientation, glm::vec3(0.0f, 0.0f, 1.0f));
	translateMatrix = glm::translate(glm::mat4(1.0f), position);
}

void OBJObject::incrementPointSize(GLfloat offset) {
	pointSize += offset;

	if (pointSize < 1.0f) {
		pointSize = 1.0f;
	}
}
