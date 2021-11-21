#include "Box.h"

vec2 box(vec3 ro, vec3 rd, vec3 boxSize, vec3& outNormal) {
	vec3 m = vec3(1.0f) / rd;
	vec3 n = m * ro;
	vec3 k = abs(m) * boxSize;
	vec3 t1 = -n - k;
	vec3 t2 = -n + k;
	double tN = fmax(fmax(t1.x, t1.y), t1.z);
	double tF = fmin(fmin(t2.x, t2.y), t2.z);
	if (tN > tF || tF < 0.0f) return vec2(-1.0f);
	vec3 yzx = vec3(t1.y, t1.z, t1.x);
	vec3 zxy = vec3(t1.z, t1.x, t1.y);
	outNormal = -sign(rd) * step(yzx, t1) * step(zxy, t1);
	return vec2(tN, tF);
}