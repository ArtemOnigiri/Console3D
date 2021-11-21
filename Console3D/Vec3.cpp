#include <math.h>
#include "Vec3.h"
#include "Vec2.h"
#include "OverloadedMathFuncs.h"

vec3 rotateX(vec3 a, double angle)
{
	vec3 b = a;
	b.z = a.z * cos(angle) - a.y * sin(angle);
	b.y = a.z * sin(angle) + a.y * cos(angle);
	return b;
}

vec3 rotateY(vec3 a, double angle)
{
	vec3 b = a;
	b.x = a.x * cos(angle) - a.z * sin(angle);
	b.z = a.x * sin(angle) + a.z * cos(angle);
	return b;
}

vec3 rotateZ(vec3 a, double angle)
{
	vec3 b = a;
	b.x = a.x * cos(angle) - a.y * sin(angle);
	b.y = a.x * sin(angle) + a.y * cos(angle);
	return b;
}

double length(vec3 const& v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3 norm(vec3 v) {
	return v / length(v);
}

float dot(vec3 const& a, vec3 const& b) {
	return static_cast<float>(a.x * b.x + a.y * b.y + a.z * b.z);
}

vec3 abs(vec3 const& v) {
	return vec3(fabs(v.x), fabs(v.y), fabs(v.z));
}

vec3 sign(vec3 const& v) {
	return vec3(sign(v.x), sign(v.y), sign(v.z));
}

vec3 step(vec3 const& edge, vec3 v) {
	return vec3(step(edge.x, v.x), step(edge.y, v.y), step(edge.z, v.z));
}

vec3 reflect(vec3 rd, vec3 n) {
	return rd - n * (2 * dot(n, rd));
}