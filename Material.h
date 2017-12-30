#ifndef MATERIAL
#define MATERIAL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>

class Material {

	//static fields
	static GLuint shaderProgram;
	static std::vector<GLuint> allIDs;

	//diffuse
	bool useDiffuse;
	glm::vec3 diffuse;

	//specular
	bool useSpecular;
	glm::vec3 specular;

	//ambient
	bool useAmbient;
	glm::vec3 ambient;

	//texture
	bool useTexture;
	GLuint textureID;
	float textureStrength;

	//normal map
	bool useNormalMap;
	GLuint normalMapID;
	float normalMapStrength;

	//relection texture
	bool useReflectionTexture;
	GLuint reflectionTextureID;
	float reflectiveness;

public:
	
	//manage statics
	static void initStatics();
	static void cleanUpStatics();
	static Material basic();

	Material();
	~Material();

	//diffuse
	void setUseDiffuse(bool opt);
	void setDiffuseColor(glm::vec3 c);
	glm::vec3 getDiffuseColor();

	//specular
	void setUseSpecular(bool opt);
	void setSpecularColor(glm::vec3 c);
	glm::vec3 getSpecularColor();

	//ambient
	void setUseAmbient(bool opt);
	void setAmbientColor(glm::vec3 c);
	glm::vec3 getAmbientColor();

	//texture
	void setUseTexture(bool opt);
	void loadTexture(const char* filename);
	GLuint getTextureID();
	void setTextureStrength(float f);
	float getTextureStrength();

	//normal map
	void setUseNormalMap(bool opt);
	void loadNormalMap(const char* filename);
	GLuint getNormalMapID();
	void setNormalMapStrength(float f);
	float getNormalMapStrength();

	//reflection texture
	void setUseReflectionTexture(bool opt);
	void loadReflectionTexture(std::vector<std::string> faces);
	GLuint getReflectionTextureID();
	void setReflectiveness(float r);
	float getReflectiveness();
	
	//return static material shader program for clinets to use
	static GLuint getShaderProgram();

	//sent material settings to static shader program
	void applySettings();
	
	//Helper Statics
	static void loadImage(const char* filename, GLuint &currID);
	static void loadCubeMapTexture(std::vector<std::string> faces, GLuint &currID);
	static unsigned char* loadPPM(const char* filename, int& width, int& height);
	
};

#endif