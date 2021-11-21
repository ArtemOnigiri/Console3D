#pragma once
#include "Vec2.h"

struct vec3
{
	double x, y, z;

	vec3() : x(0), y(0), z(0) {}
	vec3(double _value) : x(_value), y(_value), z(_value) {};
	vec3(double _x, vec2 const& v) : x(_x), y(v.x), z(v.y) {};
	vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};

	vec3 operator+(vec3 const& other) { return vec3(x + other.x, y + other.y, z + other.z); }
	vec3 operator-(vec3 const& other) { return vec3(x - other.x, y - other.y, z - other.z); }
	vec3 operator*(vec3 const& other) { return vec3(x * other.x, y * other.y, z * other.z); }
	vec3 operator/(vec3 const& other) { return vec3(x / other.x, y / other.y, z / other.z); }
	vec3 operator-() { return vec3(-x, -y, -z); }

};

double length(vec3 const& v);
vec3 norm(vec3 v);
float dot(vec3 const& a, vec3 const& b);
vec3 abs(vec3 const& v);
vec3 sign(vec3 const& v);
vec3 step(vec3 const& edge, vec3 v);
vec3 reflect(vec3 rd, vec3 n);

vec3 rotateX(vec3 a, double angle);
vec3 rotateY(vec3 a, double angle);
vec3 rotateZ(vec3 a, double angle);