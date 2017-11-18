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
	std::vector<glm::vec3> tangentAPoints;
	GLuint VAO_tangentA, VBO_tangentA;

	std::vector<glm::vec3> tangentBPoints;
	GLuint VAO_tangentB, VBO_tangentB;

	GLuint uProjection, uModelview;
	GLuint uToWorld;

	

	
	
public:

	static void InitStatics();
	static void cleanUpStatics();

	BezierCurve();
	BezierCurve(ControlPoint* interp1, ControlPoint* approx1, ControlPoint* approx2, ControlPoint* interp2);
	~BezierCurve();
	void draw(ControlPoint* currSelected);
	glm::vec3 positionAtTime(float t);
	void updateCurveLines();

private:
	float C_func(int i, float t);
	float fact(int n);
	
	
};