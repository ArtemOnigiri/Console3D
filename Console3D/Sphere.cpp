#include "Sphere.h"

vec2 sphere(vec3 ro, vec3 rd, float r) {
	float b = dot(ro, rd);
	float c = dot(ro, ro) - r * r;
	double h = b * b - c;
	if (h < 0.0f) return vec2(-1.0f);
	h = sqrt(h);
	return vec2(-b - h, -b + h);
}