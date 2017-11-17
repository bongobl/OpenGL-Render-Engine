#include "ControlPoint.h"
#include "shader.h"
#include <iostream>
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

ControlPoint::ControlPoint() {

}
ControlPoint::ControlPoint(glm::vec3 col) {
	position = glm::vec3(0, 0, 0);
	color = col;
	handleA = handleB = parent = NULL;
}

void ControlPoint::update() {
	visual->setToWorld(glm::translate(glm::mat4(1.0f), position));
}
void ControlPoint::draw() {

	//note, visual is static, so always set toWorld before drawing OBJObject
	visual->setToWorld(glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)));
	visual->draw(color);
}
void ControlPoint::drawAsSelected() {
	visual->setToWorld(glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.7f, 0.7f, 0.7f)));
	visual->draw(glm::vec3(1,1,1));
}
void ControlPoint::move(glm::vec3 deltaPos) {
	position += deltaPos;
	
	//if an interpolation point
	if (handleA != NULL) {
		handleA->setPosition(handleA->getPosition() + deltaPos);
	}
	if (handleB != NULL) {
		handleB->setPosition(handleB->getPosition() + deltaPos);
	}
	
	//if a handle
	if (parent != NULL) {
		if (parent->handleA == this) {
			if (parent->handleB != NULL)
				parent->handleB->setPosition(parent->getPosition() + parent->getPosition() - this->getPosition());
		}
		if (parent->handleB == this) {
			if (parent->handleA != NULL)
				parent->handleA->setPosition(parent->getPosition() + parent->getPosition() - this->getPosition());
		}
	}

}
void ControlPoint::setColor(glm::vec3 newCol) {
	color = newCol;
}

glm::vec3 ControlPoint::getPosition() {
	return position;
}

void ControlPoint::setPosition(glm::vec3 pos) {
	position = pos;
}

void ControlPoint::updateOtherHandle(ControlPoint* activeHandle) {

	if (activeHandle == handleA) {
		if(handleB != NULL)
			handleB->setPosition(this->getPosition() +  this->getPosition() - activeHandle->getPosition());
	}
}