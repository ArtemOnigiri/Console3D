#include "OverloadedMathFuncs.h"

double clamp(double value, double min, double max) {
	return fmax(fmin(value, max), min);
}

double sign(double a) {
	return (0 < a) - (a < 0);
}

double step(double edge, double x) {
	return x > edge; 
}