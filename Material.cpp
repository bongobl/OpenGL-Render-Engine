#include "Material.h"

Material::Material() {

	textureID = 0;
	normalMapID = 0;
	color = glm::vec3(1, 1, 1);
}
Material::~Material() {
	//glDeleteProgram(shaderProgram);
}
void Material::loadTexture(const char* filename) {

	loadImage(filename, textureID);
}
void Material::loadNormalMap(const char* filename) {

	loadImage(filename, normalMapID);
}
void Material::loadImage(const char* filename, GLuint &currID) {

	int twidth, theight;   // texture width/height [pixels]
	unsigned char* tdata;  // texture pixel data

						   // Load image file
	tdata = loadPPM(filename, twidth, theight);
	if (tdata == NULL) return;

	// Create ID for texture
	glGenTextures(1, &currID);

	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, currID);

	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);

	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}
unsigned char* Material::loadPPM(const char* filename, int& width, int& height) {
	
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}


GLuint Material::getTextureID() {
	return textureID;
}

GLuint Material::getNormalMapID() {
	return normalMapID;
}

void Material::setShaderProgram(GLuint sp) {
	shaderProgram = sp;
}
GLuint Material::getShaderProgram() {
	return shaderProgram;
}

void Material::setColor(glm::vec3 c) {
	color = c;
}
glm::vec3 Material::getColor() {
	return color;
}