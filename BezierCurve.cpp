#include "BezierCurve.h"

BezierCurve::BezierCurve(ControlPoint* interp1, ControlPoint* approx1, ControlPoint* approx2, ControlPoint* interp2) {
	
	p0 = interp1;
	p1 = approx1;
	p2 = approx2;
	p3 = interp2;
}

void BezierCurve::draw() {

	float prevTime = 0;
	float currTime = 0;

	for (int i = 1; i <= 1500; ++i) {
		currTime = i / 1500.0f;
		glm::vec3 pointA = positionAtTime(prevTime);
		glm::vec3 pointB = positionAtTime(currTime);
		//draw segment from pointA to pointB

		prevTime = currTime;
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