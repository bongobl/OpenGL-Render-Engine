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
	std::vector<glm::vec3> segPoints;
	GLuint VAO_curve, VBO_curve;

	//for drawing tangents
	std::vector<glm::vec3> tangentPoints;
	GLuint VAO_tangent, VBO_tangent;

	GLuint uProjection, uModelview;
	GLuint uToWorld;

	

	
	
public:

	static void InitStatics();
	static void cleanUpStatics();

	BezierCurve();
	BezierCurve(ControlPoint* interp1, ControlPoint* approx1, ControlPoint* approx2, ControlPoint* interp2);
	~BezierCurve();
	void draw();
	glm::vec3 positionAtTime(float t);
	void updateCurveLines();

private:
	float C_func(int i, float t);
	float fact(int n);
	
	
};