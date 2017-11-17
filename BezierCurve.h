#pragma once
#include "ControlPoint.h"
class BezierCurve {
	
	//static fields
	static GLuint shaderProgram;

	//member fields
	ControlPoint* p0;
	ControlPoint* p1;
	ControlPoint* p2;
	ControlPoint* p3;

	//for drawing curve
	GLuint VAO, VBO;
	GLuint uProjection, uModelview;
	GLuint uToWorld;

	std::vector<glm::vec3> segPoints;
	
	
public:

	static void InitStatics();
	static void cleanUpStatics();

	BezierCurve();
	BezierCurve(ControlPoint* interp1, ControlPoint* approx1, ControlPoint* approx2, ControlPoint* interp2);
	void draw();
	glm::vec3 positionAtTime(float t);
	void updateSegPoints();

private:
	float C_func(int i, float t);
	float fact(int n);
	
	
};