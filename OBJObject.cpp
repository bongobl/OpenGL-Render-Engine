//NOTE SOMETHING WROTE WITH std::vectors

//#define GLEW_GET_FUN
#include <iostream>
#include <string>
#include <cmath>
#include "OBJObject.h"
#include "Window.h"
using namespace std;



OBJObject::OBJObject(const char *filepath, Material m) 
{

	//read in geometry data disk
	parse(filepath);
	material = m;
	setDefaultProperties();

	

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);	
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);
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


	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!


	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.


	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	
}

OBJObject::~OBJObject() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
void OBJObject::parse(const char *filepath) 
{

	// Parsing Variables	
	FILE* fp;
	char currLine[BUFSIZ];
	GLfloat x, y, z;
	GLfloat r, g, b;
	GLuint v1, v2, v3, n1, n2, n3;

	// Variables for finding Model Center of Mass
	bool firstVertex = true;
	GLfloat lowestX, highestX, lowestY, highestY, lowestZ, highestZ;

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
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);

				//Code to calc center of mass
				if (firstVertex) {
					lowestX = highestX = x;
					lowestY = highestY = y;
					lowestZ = highestZ = z;
					firstVertex = false;
				}
				if (x > highestX)	highestX = x;
				if (x < lowestX)	lowestX = x;
				if (y > highestY)	highestY = y;
				if (y < lowestY)	lowestY = y;
				if (z > highestZ)	highestZ = z;
				if (z < lowestZ)	lowestZ = z;
			}
			else if (currLine[1] == 'n' && currLine[2] == ' ') {
				sscanf(currLine + 3, "%f %f %f", &x, &y, &z);
				GLfloat magnitude = sqrt(x * x + y * y + z * z);

				normals.push_back(x / magnitude);
				normals.push_back(y / magnitude);
				normals.push_back(z / magnitude);
			}
		}
		//process face
		else if (currLine[0] == 'f' && currLine[1] == ' ') {		
			sscanf(currLine + 2, "%i//%i %i//%i %i//%i", &v1, &n1, &v2, &n2, &v3, &n3);
			indices.push_back(v1 - 1);
			//indices.push_back(n1 - 1);
			indices.push_back(v2 - 1);
			//indices.push_back(n2 - 1);
			indices.push_back(v3 - 1);
			//indices.push_back(n3 - 1);
		}
		
	}//END FOR
	
	//define object center
	glm::vec3 objectCenterOffset;
	objectCenterOffset.x = (highestX + lowestX) / 2.0f;
	objectCenterOffset.y = (highestY + lowestY) / 2.0f;
	objectCenterOffset.z = (highestZ + lowestZ) / 2.0f;

	//use model boundaries to calc scale factor matrix
	float scaleFactor = calcScaleFactor(objectCenterOffset, highestX, highestY, highestZ);
	scaleToFit = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	centerModel = glm::translate(glm::mat4(1.0f), glm::vec3(-objectCenterOffset.x, -objectCenterOffset.y, -objectCenterOffset.z));

	cout << "num vertices: " << vertices.size() << endl;
	cout << "num normals: " << normals.size() << endl;
	cout << "num indices: " << indices.size() << endl;	

}

void OBJObject::setDefaultProperties() {
	
	//set raw Object Properties
	this->position = glm::vec3(0, 0, 0);
	this->scale = 1.0f;
	this->modelZoom = 0;

	//set transformation matrices to reflect object properties.
	translateMatrix = glm::translate(glm::mat4(1.0f), position);
	scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
	trackBallRotate = glm::mat4(1.0);
	modelZoomMatrix = glm::translate(glm::mat4(1.0f), position);
}

float OBJObject::calcScaleFactor(glm::vec3 &objectCenterOffset, float &highestX, float &highestY, float &highestZ) {
	vector<float> elems;
	elems.push_back(highestX - objectCenterOffset.x);
	elems.push_back(highestY - objectCenterOffset.y);
	elems.push_back(highestZ - objectCenterOffset.z);

	float furthest = 0;
	for (unsigned int i = 0; i < elems.size(); ++i) {
		if (elems[i] > furthest)
			furthest = elems[i];
	}

	return 1.0f / furthest;
}

void OBJObject::draw(GLuint currentShaderProgram)
{
	//ALL INFO HERE IS SENT TO VERT FILE, NOT FRAG FILE
	toWorld = modelZoomMatrix * translateMatrix * trackBallRotate * scaleMatrix * scaleToFit * centerModel;
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(currentShaderProgram, "projection");
	uModelview = glGetUniformLocation(currentShaderProgram, "modelview");
	uToWorld = glGetUniformLocation(currentShaderProgram, "toWorld");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(uToWorld, 1, GL_FALSE, &toWorld[0][0]);

	//Apply model material properties
	glUniform1f(glGetUniformLocation(currentShaderProgram, "material.diffuse"), material.diffuse);
	glUniform1f(glGetUniformLocation(currentShaderProgram, "material.specular"), material.specular);
	glUniform3f(glGetUniformLocation(currentShaderProgram, "material.materialColor"), material.color.x, material.color.y, material.color.z);
	glUniform1f(glGetUniformLocation(currentShaderProgram, "material.shine"), material.shine);

	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void OBJObject::update() {

	
}
void OBJObject::lookAt(glm::vec3 target) {
	
	//special case, use trackballRotate
	trackBallRotate = glm::inverse(glm::lookAt(position, target, glm::vec3(0, 0, 1)));
}
void OBJObject::setPosition(glm::vec3 newPosition) {
	position = newPosition;
	translateMatrix = glm::translate(glm::mat4(1.0f), position);

}

void OBJObject::move(glm::vec3 displacement) {
	position = position + displacement;
	translateMatrix = glm::translate(glm::mat4(1.0f), position);

}
void OBJObject::setScale(float scaleVal) {
	scale = scaleVal;
	scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
}
void OBJObject::incrementScale(float scaleDiff) {
	scale += scaleDiff;
	scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
}


void OBJObject::updateTrackBallRotate(glm::mat4 offset) {
	trackBallRotate = offset * trackBallRotate;
	position = glm::vec3(   offset * glm::vec4(position.x, position.y, position.z,1.0f));
	translateMatrix = glm::translate(glm::mat4(1.0f), position);
}

void OBJObject::setZoomVal(float z) {
	modelZoom = 0;
	modelZoomMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, modelZoom));
}
void OBJObject::zoomModel(float z) {
	
	/*
	modelZoom += z;
	modelZoomMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,modelZoom));
	*/
	position.z += z;
	translateMatrix = glm::translate(glm::mat4(1.0f), position);

}