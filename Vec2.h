#pragma once

struct vec2
{
    double x, y;

    vec2(double _value) : x(_value), y(_value) {};
    vec2(double _x, double _y) : x(_x), y(_y) {};

    vec2 operator+(vec2 other) { return vec2(x + other.x, y + other.y); }
    vec2 operator-(vec2 other) { return vec2(x - other.x, y - other.y); }
    vec2 operator/(vec2 other) { return vec2(x / other.x, y / other.y); }
};