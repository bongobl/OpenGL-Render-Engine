#pragma once
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "OBJObject.h"
#include "shader.h"
#include "Node.h"
#include "GeometryNode.h"
#include "TransformNode.h"
class Robot {

	//static fields or all robots
	static GLint RobotShaderProgram;
	static OBJObject* body;
	static OBJObject* head;
	static OBJObject* arm;
	static OBJObject* leg;
	static OBJObject* eye;
	static OBJObject* antenna;
	static OBJObject* sphere;

	//member fields
	TransformNode* rootNode;
	TransformNode* bodyToRoot;
	TransformNode* headToBody;
	TransformNode* rightArmToBody;
	TransformNode* leftArmToBody;
	TransformNode* rightLegToBody;
	TransformNode* leftLegToBody;
	TransformNode* rightEyeToHead;
	TransformNode* leftEyeToHead;
	TransformNode* rightAntennaToHead;
	TransformNode* leftAntennaToHead;
	TransformNode* boundingSphereToBody;

	GeometryNode* boundingSphere;

	GLfloat clock;
	GLfloat prevTime;

public:

	static void initializeStatics();
	static void cleanUpStatics();
	Robot(glm::vec3 position);
	~Robot();

	void update();
	void draw(glm::mat4 start);
	void setPosition(glm::vec3 position);
	glm::mat4 getBoundingSphereToRoot();
};

