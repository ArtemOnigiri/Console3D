#include "Plane.h"

double plane(vec3 ro, vec3 rd, vec3 p, double w) {
	return -(dot(ro, p) + w) / dot(rd, p);
}