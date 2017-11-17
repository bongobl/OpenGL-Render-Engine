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

	for (int i = 0; i <= 150; ++i) {
		segPoints.push_back(glm::vec3(0,0,0));
	}

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	updateSegPoints();

}


void BezierCurve::draw() {


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


	// Now draw this OBJObject. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);

	glDrawArrays(GL_LINE_STRIP, 0, segPoints.size());

	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);


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

void BezierCurve::updateSegPoints() {
	
	for (int i = 0; i <= 150; ++i) {
		float currTime = i / 150.0f;
		segPoints[i] = positionAtTime(currTime);
	}

	
	

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, segPoints.size() * sizeof(glm::vec3), segPoints.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindVertexArray(0);
}