#pragma once
#include "ControlPoint.h"
class BezierCurve {

	ControlPoint* p0;
	ControlPoint* p1;
	ControlPoint* p2;
	ControlPoint* p3;

	//delete me, temp for drawing curve
	ControlPoint* rider;
	
public:

	BezierCurve(ControlPoint* interp1, ControlPoint* approx1, ControlPoint* approx2, ControlPoint* interp2);
	void draw();
	glm::vec3 positionAtTime(float t);
	

private:
	float C_func(int i, float t);
	float fact(int n);
	
};