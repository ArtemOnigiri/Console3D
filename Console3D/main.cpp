#include <iostream>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include "ScreenPreparer.h"
#include "Vec2.h"
#include "Vec3.h"
#include "OverloadedMathFuncs.h"
#include "Sphere.h"
#include "Box.h"
#include "Plane.h"

int main() {
	PrepareWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

	wchar_t* screenBuffer = CreateScreenBuffer();
	HANDLE hConsole = GetConsoleHandle();

	vec3 light = norm(vec3(-0.5, 0.5, -1.0));
	vec3 spherePos = vec3(0, 3, 0);
	vec2 uv;
	vec3 ro;
	vec3 rd;

	for (size_t t = 0; t < ANIMATION_TIME; t++) {
		light = norm(vec3(-0.5, 0.5, -1.0));
		spherePos = vec3(0, 3, 0);
		for (size_t i = 0; i < WINDOW_WIDTH; i++) {
			for (size_t j = 0; j < WINDOW_HEIGHT; j++) {
				uv = vec2(i, j) / vec2(WINDOW_WIDTH, WINDOW_HEIGHT) * 2.0f - 1.0f;
				uv.x *= WINDOW_ASPECT * CONSOLE_CHAR_ASPECT;
				ro = vec3(-6, 0, 0);
				rd = norm(vec3(2, uv));
				ro = rotateY(ro, 0.25);
				rd = rotateY(rd, 0.25);
				ro = rotateZ(ro, t * 0.01);
				rd = rotateZ(rd, t * 0.01);
				double diff = 1;
				for (size_t k = 0; k < 5; k++) {
					double minIt = INT_MAX;
					vec2 intersection = sphere(ro - spherePos, rd, 1);
					vec3 n = 0;
					float albedo = 1;
					if (intersection.x > 0) {
						vec3 itPoint = ro - spherePos + rd * intersection.x;
						minIt = intersection.x;
						n = norm(itPoint);
					}
					vec3 boxN = 0;
					intersection = box(ro, rd, 1, boxN);
					if (intersection.x > 0 && intersection.x < minIt) {
						minIt = intersection.x;
						n = boxN;
					}
					intersection = plane(ro, rd, vec3(0, 0, -1), 1);
					if (intersection.x > 0 && intersection.x < minIt) {
						minIt = intersection.x;
						n = vec3(0, 0, -1);
						albedo = 0.5;
					}
					if (minIt < INT_MAX) {
						diff *= (dot(n, light) * 0.5 + 0.5) * albedo;
						ro = ro + rd * (minIt - 0.01);
						rd = reflect(rd, n);
					}
					else {
						break;
					}
				}
				size_t colorNumber = static_cast<size_t>(diff * 20);
				colorNumber = static_cast<size_t>(clamp(static_cast<double>(colorNumber), 0.f, static_cast<double>(COLORS_COUNT)));

				char pixel = CHARS_GRADIENT[colorNumber];
				screenBuffer[i + j * WINDOW_WIDTH] = pixel;
			}
		}
		RedrawScreenBuffer(hConsole, screenBuffer);
	}
}