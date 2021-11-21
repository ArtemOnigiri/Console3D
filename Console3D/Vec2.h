#pragma once

struct vec2
{
	double x, y;

	vec2() : x(0), y(0) {}
	vec2(double value) : x(value), y(value) {}
	vec2(double _x, double _y) : x(_x), y(_y) {}

	vec2 operator+(vec2 const& other) { return vec2(x + other.x, y + other.y); }
	vec2 operator-(vec2 const& other) { return vec2(x - other.x, y - other.y); }
	vec2 operator*(vec2 const& other) { return vec2(x * other.x, y * other.y); }
	vec2 operator/(vec2 const& other) { return vec2(x / other.x, y / other.y); }
};

double length(vec2 const& v);