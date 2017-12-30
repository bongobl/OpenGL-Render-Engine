#include "Material.h"
#include "shader.h"

GLuint Material::shaderProgram = -1;

void Material::initStatics() {
	shaderProgram = LoadShaders("../shader.vert", "../shader_material.frag");

}
void Material::cleanUpStatics() {

	glDeleteProgram(shaderProgram);
}

Material Material::basic() {
	return Material();
}
Material::Material() {

	//diffuse
	useDiffuse = false;
	diffuse = glm::vec3(0, 0, 0);

	//specular
	useSpecular = false;
	specular = glm::vec3(0, 0, 0);

	//ambient
	useAmbient = false;
	ambient = glm::vec3(0, 0, 0);

	//surface texture
	useSurfaceTexture = false;
	surfaceTextureStrength = 1.0f;

	//normal map
	useNormalMap = false;
	normalMapStrength = 1.0f;

	//relection texture
	useReflectionTexture = false;
	reflectiveness = 1.0f;

}
Material::~Material() {
	
}

//diffuse
void Material::setUseDiffuse(bool opt) {
	useDiffuse = opt;
}
void Material::setDiffuseColor(glm::vec3 c) {
	diffuse = c;
}
glm::vec3 Material::getDiffuseColor() {
	return diffuse;
}

//specular
void Material::setUseSpecular(bool opt) {
	useSpecular = opt;
}
void Material::setSpecularColor(glm::vec3 c) {
	specular = c;
}
glm::vec3 Material::getSpecularColor() {
	return specular;
}

//ambient
void Material::setUseAmbient(bool opt) {
	useAmbient = opt;
}
void Material::setAmbientColor(glm::vec3 c) {
	ambient = c;
}
glm::vec3 Material::getAmbientColor() {
	return ambient;
}

//surface texture
void Material::setUseSurfaceTexture(bool opt) {
	if (opt == true && surfaceTexture.getType() != Texture::STANDARD) {
		std::cerr << "ERROR: No texture map loaded" << std::endl;
		return;
	}
	useSurfaceTexture = opt;
}
void Material::loadSurfaceTexture(Texture surface_texture) {

	if (surface_texture.getType() != Texture::STANDARD) {
		std::cerr << "ERROR: Texture type must be standard" << std::endl;
		return;
	}
	surfaceTexture = surface_texture;
}
Texture& Material::getSurfaceTexture() {
	return surfaceTexture;
}
void Material::setSurfaceTextureStrength(float f) {
	surfaceTextureStrength = f;
}
float Material::getSurfaceTextureStrength() {
	return surfaceTextureStrength;
}

//normal map
void Material::setUseNormalMap(bool opt) {
	if (opt == true && normalMap.getType() != Texture::STANDARD) {
		std::cerr << "ERROR: No normal map loaded" << std::endl;
		return;
	}
	useNormalMap = opt;
}
void Material::loadNormalMap(Texture normal_map) {

	if (normal_map.getType() != Texture::STANDARD) {
		std::cerr << "ERROR: Normal Map type must be standard" << std::endl;
		return;
	}
	normalMap = normal_map;
}
Texture& Material::getNormalMap() {
	return normalMap;
}
void Material::setNormalMapStrength(float f) {
	normalMapStrength = f;
}
float Material::getNormalMapStrength() {
	return normalMapStrength;
}

//reflection texture
void Material::setUseReflectionTexture(bool opt) {
	if (opt == true && reflectionTexture.getType() != Texture::CUBE_MAP) {
		std::cerr << "ERROR: No Reflection texture loaded" << std::endl;
		return;
	}
	useReflectionTexture = opt;
}
void Material::loadReflectionTexture(Texture reflection_texture) {

	if (reflection_texture.getType() != Texture::CUBE_MAP) {
		std::cerr << "ERROR: Reflection Texture type must be cube map" << std::endl;
		return;
	}
	reflectionTexture = reflection_texture;
}
Texture& Material::getReflectionTexture() {
	return reflectionTexture;
}
void Material::setReflectiveness(float r) {
	reflectiveness = r;
}
float Material::getReflectiveness() {
	return reflectiveness;
}

//static shader program for others to use
GLuint Material::getShaderProgram() {
	return shaderProgram;
}


void Material::applySettings() {

	//material properties	
	glUniform1i(glGetUniformLocation(shaderProgram, "material.useDiffuse"), useDiffuse);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.useSpecular"), useSpecular);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.useAmbient"), useAmbient);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.useSurfaceTexture"), useSurfaceTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.useNormalMap"), useNormalMap);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.useReflectionTexture"), useReflectionTexture);
	if (useDiffuse) {
		glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), diffuse.r, diffuse.g, diffuse.b);
	}
	if (useSpecular) {
		glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), specular.r, specular.g, specular.b);
	}
	if (useAmbient) {
		glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), ambient.r, ambient.g, ambient.b);
	}
	if (useSurfaceTexture) {
		glUniform1i(glGetUniformLocation(shaderProgram, "material.surfaceTexture"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, surfaceTexture.getID());

		glUniform1f(glGetUniformLocation(shaderProgram, "material.surfaceTextureStrength"), surfaceTextureStrength);
	}
	if (useNormalMap) {
		glUniform1i(glGetUniformLocation(shaderProgram, "material.normalMap"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalMap.getID());

		glUniform1f(glGetUniformLocation(shaderProgram, "material.normalMapStrength"), normalMapStrength);
	}
	
	if (useReflectionTexture) {
		glUniform1i(glGetUniformLocation(shaderProgram, "material.reflectionTexture"), 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, reflectionTexture.getID());

		glUniform1f(glGetUniformLocation(shaderProgram, "material.reflectiveness"), reflectiveness);
	}
}
