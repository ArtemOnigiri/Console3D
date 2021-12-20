#pragma once
#include <math.h>
#include "Vec2.h"
#include "Vec3.h"
#include <vector>

double clamp(double value, double min, double max) { return fmax(fmin(value, max), min); }
double sign(double a) { return (0 < a) - (a < 0); }
double step(double edge, double x) { return x > edge; }

double length(vec2 const& v) { return sqrt(v.x * v.x + v.y * v.y); }

double length(vec3 const& v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }
vec3 norm(vec3 v) { return v / length(v); }
double dot(vec3 const& a, vec3 const& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
vec3 abs(vec3 const& v) { return vec3(fabs(v.x), fabs(v.y), fabs(v.z)); }
vec3 sign(vec3 const& v) { return vec3(sign(v.x), sign(v.y), sign(v.z)); }
vec3 step(vec3 const& edge, vec3 v) { return vec3(step(edge.x, v.x), step(edge.y, v.y), step(edge.z, v.z)); }
vec3 reflect(vec3 rd, vec3 n) { return rd - n * (2 * dot(n, rd)); }
vec3 mix(vec3 x, vec3 y, double a) { return x * vec3(1 - a) + y * vec3(a); }

vec3 cross(vec3 a, vec3 b) {
    vec3 r = vec3(0);
    r.x = a.y * b.z - a.z * b.y;
    r.y = -(a.x * b.z - a.z * b.x);
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

vec3 refract(vec3 i, vec3 n, double eta) {
    double k = 1.0 - eta * eta * (1.0 - dot(n, i) * dot(n, i));
    vec3 r = vec3(0.0);
    if (k >= 0.0)
        r = vec3(eta) * i - vec3((eta * dot(n, i) + sqrt(k))) * n;
    return r;
}

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

vec2 sphere(vec3 ro, vec3 rd, double r) {
    double b = dot(ro, rd);
    double c = dot(ro, ro) - r * r;
    double h = b * b - c;
    if (h < 0.0) return vec2(-1.0);
    h = sqrt(h);
    return vec2(-b - h, -b + h);
}

vec2 box(vec3 ro, vec3 rd, vec3 boxSize, vec3& outNormal) {
    vec3 m = vec3(1.0) / rd;
    vec3 n = m * ro;
    vec3 k = abs(m) * boxSize;
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;
    double tN = fmax(fmax(t1.x, t1.y), t1.z);
    double tF = fmin(fmin(t2.x, t2.y), t2.z);
    if (tN > tF || tF < 0.0) return vec2(-1.0);
    vec3 yzx = vec3(t1.y, t1.z, t1.x);
    vec3 zxy = vec3(t1.z, t1.x, t1.y);
    outNormal = -sign(rd) * step(yzx, t1) * step(zxy, t1);
    return vec2(tN, tF);
}

double plane(vec3 ro, vec3 rd, vec3 p, double w) {
    return -(dot(ro, p) + w) / dot(rd, p);
}

vec3 triangle(vec3 ro, vec3 rd, vec3 v0, vec3 v1, vec3 v2) {
    vec3 v1v0 = v1 - v0;
    vec3 v2v0 = v2 - v0;
    vec3 rov0 = ro - v0;
    vec3 n = cross(v1v0, v2v0);
    vec3 q = cross(rov0, rd);
    double d = 1.0 / dot(rd, n);
    double u = d * dot(-q, v2v0);
    double v = d * dot(q, v1v0);
    double t = d * dot(-n, rov0);
    if (u < 0.0 || u > 1.0 || v < 0.0 || (u + v) > 1.0) {
        t = -1.0;
    }
    return vec3(t, u, v);
}

void Prism(int faces, double h, double ra, vec3 pos, std::vector<std::vector<vec3>> &triangles) {
    double angle = 360 / faces * (3.1415926 / 180);
    for (int i = 0; i < faces; i++) {
        double x1 = pos.x + ra * cos(angle * i);
        double y1 = pos.y + ra * sin(angle * i);
        double x2 = pos.x + ra * cos(angle * (i + 1));
        double y2 = pos.y + ra * sin(angle * (i + 1));
        triangles[i] = std::vector<vec3>({ vec3(pos.x, pos.y, pos.z + h), vec3(x1, y1, pos.z + h), vec3(x2, y2, pos.z + h) });
    }
    for (int i = 0; i < faces; i++) {
        double x1 = pos.x + ra * cos(angle * i);
        double y1 = pos.y + ra * sin(angle * i);
        double x2 = pos.x + ra * cos(angle * (i + 1));
        double y2 = pos.y + ra * sin(angle * (i + 1));
        triangles[i + faces * 3] = std::vector<vec3>({ vec3(pos.x, pos.y, pos.z), vec3(x2, y2, pos.z), vec3(x1, y1, pos.z) });
    }
    for (int i = 0; i < faces * 2; i++) {
        if (i < faces) {
            triangles[i + faces] = std::vector<vec3>({ triangles[i + faces * 3][1], triangles[i][2], triangles[i][1] });
        }
        else {
            triangles[i + faces] = std::vector<vec3>({ triangles[i - faces][1], triangles[i + faces * 2][2], triangles[i + faces * 2][1] });
        }
    }
}

void Pyramid(int faces, double h, double ra, vec3 pos, std::vector<std::vector<vec3>> &triangles) {
    double angle = 360 / faces * (3.1415926 / 180);
    for (int i = 0; i < faces; i++) {
        double x1 = pos.x + ra * cos(angle * i);
        double y1 = pos.y + ra * sin(angle * i);
        double x2 = pos.x + ra * cos(angle * (i + 1));
        double y2 = pos.y + ra * sin(angle * (i + 1));
        triangles[i] = std::vector<vec3>({ vec3(pos.x, pos.y, pos.z), vec3(x1, y1, pos.z), vec3(x2, y2, pos.z) });
    }
    for (int i = 0; i < faces; i++) {
        triangles[i + faces] = std::vector<vec3>({ triangles[i][1], triangles[i][2], vec3(pos.x, pos.y, pos.z + h) });
    }
}