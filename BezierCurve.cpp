#include "BezierCurve.h"
#include "shader.h"
#include "Window.h"
GLuint BezierCurve::shaderProgram;

void BezierCurve::InitStatics() {
	shaderProgram = LoadShaders("../shader.vert", "../shader_bezier.frag");
}
void BezierCurve::cleanUpStatics() {
	glDeleteProgram(shaderProgram);
}
BezierCurve::BezierCurve() {

}
BezierCurve::BezierCurve(ControlPoint* interp1, ControlPoint* approx1, ControlPoint* approx2, ControlPoint* interp2) {
	
	p0 = interp1;
	p1 = approx1;
	p2 = approx2;
	p3 = interp2;


	p0->handleB = p1;
	p3->handleA = p2;
	p1->parent = p0;
	p2->parent = p3;


	//curve segment points
	for (int i = 0; i <= 150; ++i) {
		segPoints.push_back(glm::vec3(0,0,0));
	}
	glGenVertexArrays(1, &VAO_curve);
	glGenBuffers(1, &VBO_curve);

	//tangent points
	for (int i = 0; i < 2; ++i)
		tangentAPoints.push_back(glm::vec3(0, 0, 0));
	glGenVertexArrays(1, &VAO_tangentA);
	glGenBuffers(1, &VBO_tangentA);
	for (int i = 0; i < 2; ++i)
		tangentBPoints.push_back(glm::vec3(0, 0, 0));
	glGenVertexArrays(1, &VAO_tangentB);
	glGenBuffers(1, &VBO_tangentB);

	updateCurveLines();

}

BezierCurve::~BezierCurve() {

	//cout << "Descructor Called" << endl;
	//glDeleteVertexArrays(1, &VAO_curve);
	//glDeleteBuffers(1, &VBO_curve);
	//glDeleteVertexArrays(1, &VAO_tangent);
	//glDeleteBuffers(1, &VBO_tangent);
}
void BezierCurve::draw(ControlPoint* currSelected) {


	glUseProgram(shaderProgram);

	//world coordinates of all segPoints is (0,0,0)
	glm::mat4 toWorld(1.0f);
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	uToWorld = glGetUniformLocation(shaderProgram, "toWorld");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(uToWorld, 1, GL_FALSE, &toWorld[0][0]);

	//set line width for all lines
	glLineWidth(2);

	// Now draw curve segment points by binding VAO_curve
	glUniform3f(glGetUniformLocation(shaderProgram, "inColor"), 0, 0, 0);
	glBindVertexArray(VAO_curve);
	glDrawArrays(GL_LINE_STRIP, 0, segPoints.size());
	glBindVertexArray(0);

	// Now draw tangents by binding VAO_tangent

	if (currSelected == p3 || currSelected == p3->handleA || currSelected == p3->handleB) {
		glUniform3f(glGetUniformLocation(shaderProgram, "inColor"), 1, 1, 0);
		glBindVertexArray(VAO_tangentA);
		glDrawArrays(GL_LINES, 0, tangentAPoints.size());
		glBindVertexArray(0);
	}
	if (currSelected == p0 || currSelected == p0->handleA || currSelected == p0->handleB) {
		glUniform3f(glGetUniformLocation(shaderProgram, "inColor"), 1, 1, 0);
		glBindVertexArray(VAO_tangentB);
		glDrawArrays(GL_LINES, 0, tangentBPoints.size());
		glBindVertexArray(0);
	}


}
glm::vec3 BezierCurve::positionAtTime(float t) {
	glm::vec3 q;

	q = C_func(0, t) * p0->getPosition() +
		C_func(1, t) * p1->getPosition() +
		C_func(2, t) * p2->getPosition() +
		C_func(3, t) * p3->getPosition();

	return q;
}


float BezierCurve::C_func(int i, float t) {
	float fraction = fact(3) / (fact(3 - i) * fact(i));
	float power = pow(t, i);
	float otherPower = pow(1 - t, 3 - i);
	return fraction * power * otherPower;
}
float BezierCurve::fact(int n) {
	if (n == 0)
		return 1;
	else
		return n * fact(n - 1);
}

void BezierCurve::updateCurveLines() {
	
	maxPoint_T = 0;

	//update curve segment poinnts
	for (int i = 0; i <= 150; ++i) {
		float currTime = i / 150.0f;
		segPoints[i] = positionAtTime(currTime);

		if (positionAtTime(currTime).y > positionAtTime(maxPoint_T).y)
			maxPoint_T = currTime;
	}

	glBindVertexArray(VAO_curve);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_curve);

	glBufferData(GL_ARRAY_BUFFER, segPoints.size() * sizeof(glm::vec3), segPoints.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindVertexArray(0);


	//update tangent lines
	tangentAPoints[0] = p3->getPosition();
	tangentAPoints[1] = p2->getPosition();
	

	glBindVertexArray(VAO_tangentA);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_tangentA);
	glBufferData(GL_ARRAY_BUFFER, tangentAPoints.size() * sizeof(glm::vec3), tangentAPoints.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);


	tangentBPoints[0] = p0->getPosition();
	tangentBPoints[1] = p1->getPosition();

	glBindVertexArray(VAO_tangentB);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_tangentB);
	glBufferData(GL_ARRAY_BUFFER, tangentBPoints.size() * sizeof(glm::vec3), tangentBPoints.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}
/*
float BezierCurve::curveLength() {

	float runningCurveLength = 0;
	//make sure this is called right after updateCurveLines()
	for (int i = 1; i <= 150; ++i) {

		float deltaX = segPoints[i].x - segPoints[i - 1].x;
		float deltaY = segPoints[i].y - segPoints[i - 1].y;
		float deltaZ = segPoints[i].z - segPoints[i - 1].z;
		float segLength = (float)sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
		runningCurveLength += segLength;
	}
	return runningCurveLength;

}
*/
float BezierCurve::paramTDistance(float t) {

	if (t < 0.001f) {
		float deltaX = positionAtTime(0).x - positionAtTime(0.001f).x;
		float deltaY = positionAtTime(0).y - positionAtTime(0.001f).y;
		float deltaZ = positionAtTime(0).z - positionAtTime(0.001f).z;
		return (float)sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
	}

	float preT = t - 0.001f;
	float deltaX = positionAtTime(t).x - positionAtTime(t - 0.001f).x;
	float deltaY = positionAtTime(t).y - positionAtTime(t - 0.001f).y;
	float deltaZ = positionAtTime(t).z - positionAtTime(t - 0.001f).z;
	return (float)sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
}

float BezierCurve::getMaxPoint_T() {

	return maxPoint_T;
}