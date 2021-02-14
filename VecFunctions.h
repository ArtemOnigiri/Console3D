#pragma once
#include <math.h>
#include "Vec2.h"
#include "Vec3.h"

double sign(double a) { return (0 < a) - (a < 0); }
double step(double edge, double x) { return x > edge; }
double length(vec2 v) { return sqrt(v.x * v.x + v.y * v.y); }
double length(vec3 v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }
vec2 norm(vec2 v) { return v / length(v); }
vec3 norm(vec3 v) { return v / length(v); }
double dot(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
vec3 abs(vec3 v) { return vec3(fabs(v.x), fabs(v.y), fabs(v.z)); }
vec3 sign(vec3 v) { return vec3(sign(v.x), sign(v.y), sign(v.z)); }
vec3 step(vec3 edge, vec3 v) { return vec3(step(edge.x, v.x), step(edge.y, v.y), step(edge.z, v.z)); }

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

vec2 sphIntersect(vec3 ro, vec3 rd, vec3 ce, double ra)
{
    vec3 oc = ro - ce;
    double b = dot(oc, rd);
    double c = dot(oc, oc) - ra * ra;
    double h = b * b - c;
    if (h < 0.0) return vec2(-1.0); // no intersection
    h = sqrt(h);
    return vec2(-b - h, -b + h);
}

vec2 boxIntersection(vec3 ro, vec3 rd, vec3 boxSize, vec3& outNormal)
{
    vec3 m = vec3(1.0) / rd; // can precompute if traversing a set of aligned boxes
    vec3 n = m * ro;   // can precompute if traversing a set of aligned boxes
    vec3 k = abs(m) * boxSize;
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;
    float tN = fmax(fmax(t1.x, t1.y), t1.z);
    float tF = fmin(fmin(t2.x, t2.y), t2.z);
    if (tN > tF || tF < 0.0) return vec2(-1.0); // no intersection
    vec3 yzx = vec3(t1.y, t1.z, t1.x);
    vec3 zxy = vec3(t1.z, t1.x, t1.y);
    outNormal = -sign(rd) * step(yzx, t1) * step(zxy, t1);
    return vec2(tN, tF);
}