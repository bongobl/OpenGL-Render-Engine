#include "GeometryNode.h"


GeometryNode::GeometryNode(const char* filepath, GLuint sp) {

	model = new OBJObject(filepath, sp);
}

void GeometryNode::draw(glm::mat4 C) {

	model->setToWorld(C);
	model->draw();
}
void GeometryNode::update() {

}

void GeometryNode::setCenter(float x, float y, float z) {
	model->modelCenter = glm::vec3(x,y,z);
}