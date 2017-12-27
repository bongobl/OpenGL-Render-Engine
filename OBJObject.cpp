#include <iostream>
#include <string>
#include <cmath>
#include "OBJObject.h"
#include "Scene.h"

using namespace std;


OBJObject::OBJObject(const char *filepath, Material m) 
{
	//read in geometry data disk
	parse(filepath);
	material = m;

	modelCenter = glm::vec3(0, 0, 0);
	toWorld = glm::mat4(1.0f);

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);	
	glGenBuffers(1, &VBO_positions);
	glGenBuffers(1, &VBO_normals);
	glGenBuffers(1, &VBO_uvs);
	glGenBuffers(1, &VBO_tangents);
	glGenBuffers(1, &VBO_bitangents);
	glGenBuffers(1, &EBO);
	
	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);


	//Vertex Position
	glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.


	//NORMALS
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		3, 
		GL_FLOAT, 
		GL_FALSE,
		3 * sizeof(GLfloat), 
		(GLvoid*)0); 


	//UVS
	glBindBuffer(GL_ARRAY_BUFFER, VBO_uvs);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), UVs.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,
		2, 
		GL_FLOAT, 
		GL_FALSE, 
		2 * sizeof(GLfloat), 
		(GLvoid*)0); 

	//TANGENTS
	glBindBuffer(GL_ARRAY_BUFFER, VBO_tangents);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), tangents.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(
		3,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),
		(GLvoid*)0);

	//BITANGENTS
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bitangents);
	glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(glm::vec3), bitangents.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(
		4,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),
		(GLvoid*)0);


	// We've sent the vertex data over to OpenGL, but there's still something missing.
	// In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), indices.data(), GL_STATIC_DRAW);


	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

OBJObject::~OBJObject() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO_positions);
	glDeleteBuffers(1, &VBO_normals);
	glDeleteBuffers(1, &VBO_uvs);
	glDeleteBuffers(1, &VBO_tangents);
	glDeleteBuffers(1, &VBO_bitangents);
	glDeleteBuffers(1, &EBO);
}
void OBJObject::parse(const char *filepath) 
{

	// Parsing Variables	
	FILE* fp;
	char currLine[BUFSIZ];
	GLfloat x, y, z;
	GLfloat r, g, b;
	GLuint v1, v2, v3, n1, n2, n3, t1, t2, t3;

	GLfloat lowestX, highestX, lowestY, highestY, lowestZ, highestZ;

	// Variables for finding Model Center of Mass
	bool firstVertex = true;

	fp = fopen(filepath, "rb");
	if (fp == NULL) {
		cerr << "error loading file" << endl;
		exit(-1);
	}
	cout << "Parsing " << string(filepath) << "..."<< endl;

	while (fgets(currLine, BUFSIZ, fp) != NULL) {

		//Process vertex attributes
		if (currLine[0] == 'v') {

			//process vertex position
			if (currLine[1] == ' ') {
				sscanf(currLine + 2, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
				vertices.push_back(glm::vec3(x,y,z));

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

			//process vertex normal
			else if (currLine[1] == 'n' && currLine[2] == ' ') {
				sscanf(currLine + 3, "%f %f %f", &x, &y, &z);
				GLfloat magnitude = sqrt(x * x + y * y + z * z);

				normals.push_back(glm::vec3(x / magnitude, y / magnitude, z / magnitude));

			}

			//process vertex texture UVs
			else if (currLine[1] == 't' && currLine[2] == ' ') {
				sscanf(currLine + 3, "%f %f", &x, &y);
				UVs.push_back(glm::vec2(x, y));
			}
		}
		//process face
		else if (currLine[0] == 'f' && currLine[1] == ' ') {		
			sscanf(currLine + 2, "%i/%i/%i %i/%i/%i %i/%i/%i", &v1, &n1, &t1, &v2, &n2, &t2, &v3, &n3, &t3);
			indices.push_back(v1 - 1);
			indices.push_back(v2 - 1);
			indices.push_back(v3 - 1);
		}
		
	}//END FOR
	
	 //define object center
	glm::vec3 meshCenterOffset;
	meshCenterOffset.x = (highestX + lowestX) / 2.0f;
	meshCenterOffset.y = (highestY + lowestY) / 2.0f;
	meshCenterOffset.z = (highestZ + lowestZ) / 2.0f;
	centerModelMesh = glm::translate(glm::mat4(1.0f), glm::vec3(-meshCenterOffset.x, -meshCenterOffset.y, -meshCenterOffset.z));

	cout << "num vertices: " << vertices.size() << endl;
	cout << "num normals: " << normals.size() << endl;
	cout << "num texCoords: " << UVs.size() << endl;
	cout << "num indices: " << indices.size() << endl;	
	
	//Calc Tangents and Bitangents
	for (unsigned int i = 0; i< vertices.size(); i += 3) {

		// Shortcuts for vertices
		glm::vec3 & v0 = vertices[i + 0];
		glm::vec3 & v1 = vertices[i + 1];
		glm::vec3 & v2 = vertices[i + 2];

		// Shortcuts for UVs
		glm::vec2 & uv0 = UVs[i + 0];
		glm::vec2 & uv1 = UVs[i + 1];
		glm::vec2 & uv2 = UVs[i + 2];

		// Edges of the triangle : postion delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		// Same thing for binormals
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);

	}//END FOR
}

void OBJObject::draw(Scene* currScene) {

	glUseProgram(Material::getShaderProgram());

	Camera* activeCamera = currScene->getActiveCamera();

	glm::mat4 modelCenterMatrix = glm::translate(glm::mat4(1.0f), modelCenter);
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = activeCamera->ViewMatrix * toWorld;

	GLuint uProjection = glGetUniformLocation(Material::getShaderProgram(), "projection");
	GLuint uModelview = glGetUniformLocation(Material::getShaderProgram(), "modelview");
	GLuint uToWorld = glGetUniformLocation(Material::getShaderProgram(), "toWorld");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &activeCamera->ProjectionMatrix[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(uToWorld, 1, GL_FALSE, &toWorld[0][0]);

	//send camera properties to shader program
	glUniform3f(glGetUniformLocation(Material::getShaderProgram(), "camPosition"), activeCamera->position.x, activeCamera->position.y, activeCamera->position.z);

	// Now draw this OBJObject. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


	//apply light properties
	glm::vec3 lightDirection(1, 1, 1);
	glm::vec3 lightColor(1, 1, 1);
	glUniform3f(glGetUniformLocation(Material::getShaderProgram(), "directionalLight.direction"), lightDirection.x, lightDirection.y, lightDirection.z);
	glUniform3f(glGetUniformLocation(Material::getShaderProgram(), "directionalLight.color"), lightColor.r, lightColor.g, lightColor.b);

	//material properties
	material.applySettings();


	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void OBJObject::setToWorld(glm::mat4 M_new) {
	this->toWorld = M_new;
}
void OBJObject::setMaterial(Material m) {
	material = m;
}

void OBJObject::setModelCenter(glm::vec3 newCenter) {
	modelCenter = newCenter;
}

std::vector<glm::vec3> OBJObject::getVertices() {
	return vertices;
}