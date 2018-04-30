#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include "Texture.h"

class Material {

	//static fields
	static GLuint shaderProgram;

	//diffuse
	int useDiffuse;
	glm::vec3 diffuse;

	//specular
	int useSpecular;
	glm::vec3 specular;

	//ambient
	int useAmbient;
	glm::vec3 ambient;

	//surface color
	int useSurfaceColor;
	glm::vec3 surfaceColor;

	//surface texture
	int useSurfaceTexture;
	Texture surfaceTexture;
	float surfaceTextureStrength;
	
	//normal map
	int useNormalMap;
	float normalMapStrength;
	Texture normalMap;

	//relection texture
	int useReflectionTexture;
	Texture reflectionTexture;
	float reflectiveness;
	

public:
	
	//manage statics
	static void initStatics();
	static void cleanUpStatics();

	Material();
	~Material();

	//diffuse
	void setUseDiffuse(int opt);
	void setDiffuseColor(glm::vec3 c);
	glm::vec3 getDiffuseColor();

	//specular
	void setUseSpecular(int opt);
	void setSpecularColor(glm::vec3 c);
	glm::vec3 getSpecularColor();

	//ambient
	void setUseAmbient(int opt);
	void setAmbientColor(glm::vec3 c);
	glm::vec3 getAmbientColor();

	//surface color
	void setUseSurfaceColor(int opt);
	void setSurfaceColor(glm::vec3 surface_color);
	glm::vec3 getSurfaceColor();

	//surface texture
	void setUseSurfaceTexture(int opt);
	void loadSurfaceTexture(Texture surface_texture);
	Texture& getSurfaceTexture();
	void setSurfaceTextureStrength(float f);
	float getSurfaceTextureStrength();

	//normal map
	void setUseNormalMap(int opt);
	void loadNormalMap(Texture normal_map);
	Texture& getNormalMap();
	void setNormalMapStrength(float f);
	float getNormalMapStrength();

	//reflection texture
	void setUseReflectionTexture(int opt);
	void loadReflectionTexture(Texture reflection_texture);
	Texture& getReflectionTexture();
	void setReflectiveness(float r);
	float getReflectiveness();
	
	//return static material shader program for clinets to use
	static GLuint getShaderProgram();

	//sent material settings to static shader program
	void applySettings();

};
