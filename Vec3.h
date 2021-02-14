#pragma once
#include "Vec2.h"

struct vec3
{
    double x, y, z;

    vec3(double _value) : x(_value), y(_value), z(_value) {};
    vec3(vec2 v, double _z) : x(v.x), y(v.y), z(_z) {};
    vec3(double _x, vec2 v) : x(_x), y(v.x), z(v.y) {};
    vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};

    vec3 operator+(vec3 other) { return vec3(x + other.x, y + other.y, z + other.z); }
    vec3 operator-(vec3 other) { return vec3(x - other.x, y - other.y, z - other.z); }
    vec3 operator*(vec3 other) { return vec3(x * other.x, y * other.y, z * other.z); }
    vec3 operator/(vec3 other) { return vec3(x / other.x, y / other.y, z / other.z); }
    vec3 operator-() { return vec3(-x, -y, -z); }

	vec3 xxx() { return vec3(x, x, x); }
	vec3 xxy() { return vec3(x, x, y); }
	vec3 xxz() { return vec3(x, x, z); }
	vec3 xyx() { return vec3(x, y, x); }
	vec3 xyy() { return vec3(x, y, y); }
	vec3 xyz() { return vec3(x, y, z); }
	vec3 xzx() { return vec3(x, z, x); }
	vec3 xzy() { return vec3(x, z, y); }
	vec3 xzz() { return vec3(x, z, z); }
	vec3 yxx() { return vec3(y, x, x); }
	vec3 yxy() { return vec3(y, x, y); }
	vec3 yxz() { return vec3(y, x, z); }
	vec3 yyx() { return vec3(y, y, x); }
	vec3 yyy() { return vec3(y, y, y); }
	vec3 yyz() { return vec3(y, y, z); }
	vec3 yzx() { return vec3(y, z, x); }
	vec3 yzy() { return vec3(y, z, y); }
	vec3 yzz() { return vec3(y, z, z); }
	vec3 zxx() { return vec3(z, x, x); }
	vec3 zxy() { return vec3(z, x, y); }
	vec3 zxz() { return vec3(z, x, z); }
	vec3 zyx() { return vec3(z, y, x); }
	vec3 zyy() { return vec3(z, y, y); }
	vec3 zyz() { return vec3(z, y, z); }
	vec3 zzx() { return vec3(z, z, x); }
	vec3 zzy() { return vec3(z, z, y); }
	vec3 zzz() { return vec3(z, z, z); }

};