#include "TransformNode.h"

TransformNode::TransformNode() {
	
	this->M = glm::mat4(1.0f);
	this->position = glm::vec3(0, 0, 0);
	this->scale = 1;
	this->rotation = glm::mat4(1.0f);
}

void TransformNode::draw(glm::mat4 C) {
	glm::mat4 M_new = C * this->M;
	for (auto iter = children.begin(); iter != children.end(); ++iter) {
		(*iter)->draw(M_new);
	}
}
void TransformNode::update() {

}
void TransformNode::addChild(Node* newChild) {
	children.push_back(newChild);
}

void TransformNode::removeChild(Node* childToRemove) {
	children.remove(childToRemove);
}

void TransformNode::setPosition(glm::vec3 pos) {
	position = pos;
	update_M();
}

void TransformNode::setScale(float s) {
	scale = s;
	update_M();
}

void TransformNode::setRotation(glm::mat4 rot) {
	rotation = rot;
	update_M();
}

void TransformNode::update_M() {
	M = glm::translate(glm::mat4(1.0f), position)
		* rotation * 
		glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
}