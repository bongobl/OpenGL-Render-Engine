#include "ControlPoint.h"
#include "shader.h"

OBJObject* ControlPoint::visual;
GLuint ControlPoint::PointShaderProgram;

void ControlPoint::InitStatics() {
	PointShaderProgram = LoadShaders("../shader.vert", "../shader_point.frag");
	visual = new OBJObject("sphere.obj", PointShaderProgram);
}

void ControlPoint::cleanUpStatics() {
	delete visual;
	glDeleteProgram(PointShaderProgram);
}

ControlPoint::ControlPoint(glm::vec3 col) {
	position = glm::vec3(0, 0, 0);
	color = col;
}

void ControlPoint::update() {
	visual->setToWorld(glm::translate(glm::mat4(1.0f), position));
}
void ControlPoint::draw() {

	//note, visual is static, so always set toWorld before drawing OBJObject
	visual->setToWorld(glm::translate(glm::mat4(1.0f), position));
	visual->draw(color);
}
void ControlPoint::drawAsSelected() {
	visual->setToWorld(glm::translate(glm::mat4(1.0f), position));
	visual->draw(glm::vec3(1,1,1));
}
void ControlPoint::move(glm::vec3 deltaPos) {
	position += deltaPos;
}
void ControlPoint::setColor(glm::vec3 newCol) {
	color = newCol;
}

glm::vec3 ControlPoint::getPosition() {
	return position;
}