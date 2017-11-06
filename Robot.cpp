#include "Robot.h"

GLint Robot::RobotShaderProgram;
OBJObject* Robot::body;
OBJObject* Robot::head;
OBJObject* Robot::arm;
OBJObject* Robot::leg;
OBJObject* Robot::eye;
OBJObject* Robot::antenna;

void Robot::initializeStatics() {

	RobotShaderProgram =  LoadShaders("../shader.vert", "../shader_robot.frag");
	body = new OBJObject("robot-parts/body.obj", RobotShaderProgram);
	head = new OBJObject("robot-parts/head.obj", RobotShaderProgram);
	arm = new OBJObject("robot-parts/limb.obj", RobotShaderProgram);
	arm->setModelCenter(glm::vec3(0, 0, 10));
	leg = new OBJObject("robot-parts/limb.obj", RobotShaderProgram);
	eye = new OBJObject("robot-parts/eyeball.obj", RobotShaderProgram);
	antenna = new OBJObject("robot-parts/antenna.obj", RobotShaderProgram);

}

Robot::Robot(glm::vec3 position) {

	//initialize world Node
	worldNode = new TransformNode();

	//attach body to world
	bodyToWorld = new TransformNode();
	bodyToWorld->setPosition(position);
	bodyToWorld->setRotation(glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 2, glm::vec3(1, 0, 0)));
	bodyToWorld->addChild(new GeometryNode(body));
	worldNode->addChild(bodyToWorld);

	//attach head to body
	headToBody = new TransformNode();
	headToBody->setRotation(glm::rotate(glm::mat4(1.0f), glm::pi<float>(), glm::vec3(1, 0, 0)));
	headToBody->setPosition(glm::vec3(0, 0, -32));
	headToBody->addChild(new GeometryNode(head));
	bodyToWorld->addChild(headToBody);

	//attach right arm to body
	rightArmToBody = new TransformNode();
	rightArmToBody->setPosition(glm::vec3(-26, 0, -10));
	rightArmToBody->addChild(new GeometryNode(arm));
	bodyToWorld->addChild(rightArmToBody);

	//attach left arm to body
	leftArmToBody = new TransformNode();
	leftArmToBody->setPosition(glm::vec3(26, 0, -10));
	leftArmToBody->addChild(new GeometryNode(arm));
	bodyToWorld->addChild(leftArmToBody);

	//attach right leg to body
	rightLegToBody = new TransformNode();
	rightLegToBody->setPosition(glm::vec3(-10, 0, 20));
	rightLegToBody->addChild(new GeometryNode(leg));
	bodyToWorld->addChild(rightLegToBody);

	//attach left leg to body
	leftLegToBody = new TransformNode();
	leftLegToBody->setPosition(glm::vec3(10, 0, 20));
	leftLegToBody->addChild(new GeometryNode(leg));
	bodyToWorld->addChild(leftLegToBody);

	//attach right eye to head
	rightEyeToHead = new TransformNode();
	rightEyeToHead->setPosition(glm::vec3(-6.5f, -17, -2));
	rightEyeToHead->setScale(0.75f);
	rightEyeToHead->addChild(new GeometryNode(eye));
	headToBody->addChild(rightEyeToHead);

	//attach left eye to head
	leftEyeToHead = new TransformNode();
	leftEyeToHead->setPosition(glm::vec3(6.5f, -17, -2));
	leftEyeToHead->setScale(0.75f);
	leftEyeToHead->addChild(new GeometryNode(eye));
	headToBody->addChild(leftEyeToHead);

	//attach right antenna to head
	rightAntennaToHead = new TransformNode();
	rightAntennaToHead->setPosition(glm::vec3(-6.5f, 0, 3));
	rightAntennaToHead->addChild(new GeometryNode(antenna));
	headToBody->addChild(rightAntennaToHead);

	//attach left antenna to head
	leftAntennaToHead = new TransformNode();
	leftAntennaToHead->setPosition(glm::vec3(6.5f, 0, 3));
	leftAntennaToHead->setRotation(glm::rotate(glm::mat4(1.0f), glm::pi<float>(), glm::vec3(1, 0, 0)));
	leftAntennaToHead->addChild(new GeometryNode(antenna));
	headToBody->addChild(leftAntennaToHead);

	clock = 0;
	prevTime = (float)glfwGetTime();
}

Robot::~Robot() {

	delete worldNode;
}
void Robot::cleanUpStatics() {
	
	delete body;
	delete head;
	delete arm;
	delete leg;
	delete eye;
	delete antenna;
	glDeleteProgram(RobotShaderProgram);
}

void Robot::update() {

	float currTime = (float)glfwGetTime();
	clock += 7 * (currTime - prevTime);
	prevTime = currTime;

	leftLegToBody->setRotation(glm::rotate(glm::mat4(1.0f), sin(clock), glm::vec3(1, 0, 0)));
	rightLegToBody->setRotation(glm::rotate(glm::mat4(1.0f), -sin(clock), glm::vec3(1, 0, 0)));
	rightArmToBody->setRotation(glm::rotate(glm::mat4(1.0f), sin(clock), glm::vec3(1, 0, 0)));
	leftArmToBody->setRotation(glm::rotate(glm::mat4(1.0f), -sin(clock), glm::vec3(1, 0, 0)));
}
void Robot::draw() {
	worldNode->draw(glm::mat4(1.0f));
}

void Robot::setPosition(glm::vec3 position) {
	bodyToWorld->setPosition(position);
}