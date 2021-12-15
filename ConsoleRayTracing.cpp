#include <iostream>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <random>
#include <vector>
#include "VecFunctions.h"

void SetWindow(int Width, int Height)
{
	_COORD coord;
	coord.X = Width;
	coord.Y = Height;
	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = Height - 1;
	Rect.Right = Width - 1;
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(Handle, coord);
	SetConsoleWindowInfo(Handle, TRUE, &Rect);
}

int main() {
	int width = 240;
	int height = 68;
	int w = 2560;
	int h = 1440;
	int mouseX = w / 2;
	int mouseY = h / 2;
	SetWindow(width, height);

	double aspect = (double)width / height;
	double pixelAspect = 11.0f / 24.0f;
	char gradient[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,^ `'.";
	int gradientSize = std::size(gradient) - 2;

	double mouseSensitivity = 2.0f;
	bool wasdUD[6] = { false, false, false, false, false, false };
	double speed = 0.7f;
	vec3 pos = vec3(-6, -1, -1);

	wchar_t* screen = new wchar_t[width * height];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	int t = 0;

	while (true) {
		vec3 light = norm(vec3(-0.5, 0.5, -1.0));
		vec3 spherePos = vec3(0, 3, 0);

		POINT p;
		GetCursorPos(&p);
		double mx = p.x - w / 2;
		double my = p.y - h / 2;
		mouseX += mx;
		mouseY += my;
		SetCursorPos(w / 2, h / 2);

		if (GetAsyncKeyState(VK_ESCAPE)) break;
		else if (GetAsyncKeyState(87)) wasdUD[0] = true;
		else if (GetAsyncKeyState(65)) wasdUD[1] = true;
		else if (GetAsyncKeyState(83)) wasdUD[2] = true;
		else if (GetAsyncKeyState(68)) wasdUD[3] = true;
		else if (GetAsyncKeyState(32)) wasdUD[4] = true;
		else if (GetAsyncKeyState(67)) wasdUD[5] = true;

		mx = ((double)mouseX / w - 0.5f) * mouseSensitivity;
		my = ((double)mouseY / h - 0.5f) * mouseSensitivity;
		vec3 dir = vec3(0.0f, 0.0f, 0.0f);
		vec3 dirTemp = vec3(0);
		if (wasdUD[0]) dir = vec3(1.0f, 0.0f, 0.0f);
		else if (wasdUD[2]) dir = vec3(-1.0f, 0.0f, 0.0f);
		if (wasdUD[1]) dir = dir + vec3(0.0f, -1.0f, 0.0f);
		else if (wasdUD[3]) dir = dir + vec3(0.0f, 1.0f, 0.0f);
		dirTemp.z = dir.z * cos(-my) - dir.x * sin(-my);
		dirTemp.x = dir.z * sin(-my) + dir.x * cos(-my);
		dirTemp.y = dir.y;
		dir.x = dirTemp.x * cos(mx) - dirTemp.y * sin(mx);
		dir.y = dirTemp.x * sin(mx) + dirTemp.y * cos(mx);
		dir.z = dirTemp.z;
		pos = pos + dir * speed;
		if (wasdUD[4]) pos.z -= speed;
		else if (wasdUD[5]) pos.z += speed;

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				vec2 uv = vec2(i, j) / vec2(width, height) * 2.0f - 1.0f;
				uv.x *= aspect * pixelAspect;
				vec3 ro = pos;
				vec3 rd = norm(vec3(2, uv));
				rd = rotateY(rd, my);
				rd = rotateZ(rd, mx);
				double diff = 1;

				for (int k = 0; k < 4; k++) {
					double minIt = 99999;

					vec2 intersection = sphere(ro - spherePos, rd, 1);
					vec3 n = 0;
					double albedo = 0.0;
					if (intersection.x > 0) {
						vec3 itPoint = ro - spherePos + rd * intersection.x;
						minIt = intersection.x;
						n = norm(itPoint);
						albedo = -0.5;
					}

					vec3 boxN = 0;
					intersection = box(ro, rd, 1, boxN);
					if (intersection.x > 0 && intersection.x < minIt) {
						minIt = intersection.x;
						n = boxN;
						albedo = 1.0;
					}

					std::vector<std::vector<vec3>> prism = Prism(6, -3.0, 1.0, vec3(3, -4, 1));
					for (int a = 0; a < prism.size(); ++a) {
						intersection = vec2(triangle(ro, rd, prism[a][0], prism[a][1], prism[a][2]).x, triangle(ro, rd, prism[a][0], prism[a][1], prism[a][2]).z);
						if (intersection.x > 0) {
							minIt = intersection.x;
							vec3 v0 = prism[a][0] - prism[a][1];
							vec3 v1 = prism[a][2] - prism[a][1];
							n = norm(cross(v0, v1));
							albedo = -2.0;
						}
					}

					std::vector<std::vector<vec3>> pyramid = Pyramid(4, -2.0, 1.5, vec3(0, -5, 1));
					for (int a = 0; a < pyramid.size(); ++a) {
						intersection = vec2(triangle(ro, rd, pyramid[a][0], pyramid[a][1], pyramid[a][2]).x, triangle(ro, rd, pyramid[a][0], pyramid[a][1], pyramid[a][2]).z);
						if (intersection.x > 0) {
							minIt = intersection.x;
							vec3 v0 = pyramid[a][0] - pyramid[a][1];
							vec3 v1 = pyramid[a][2] - pyramid[a][1];
							n = norm(cross(v0, v1));
							albedo = 1.0;
						}
					}

					intersection = plane(ro, rd, vec3(0, 0, -1), 1);
					if (intersection.x > 0 && intersection.x < minIt) {
						minIt = intersection.x;
						n = vec3(0, 0, -1);
						albedo = 0.5;
					}

					if (minIt < 99999) {
						if (albedo == -2.0) diff = (double)gradientSize / 30;
						else if (albedo < 0.0) {
							double fresnel = 1.0 - abs(dot(-rd, n));
							if ((double)(rand()) / ((double)(RAND_MAX)) - 0.1 < fresnel * fresnel) {
								rd = reflect(rd, n);
							}
							else {
								ro = ro + rd * (minIt + 0.001);
								rd = refract(rd, n, 1.0 / (1.0 - albedo));
							}
						}
						else {
							diff *= (dot(n, light) * 0.5 + 0.5) * albedo;
							ro = ro + rd * (minIt - 0.01);
							rd = reflect(rd, n);
						}
					}
					else break;
				}
				int color = (int)(diff * 30);
				color = clamp(color, 0, gradientSize);
				char pixel = gradient[gradientSize - color];
				screen[i + j * width] = pixel;
			}
		}
		screen[width * height - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, width * height, { 0, 0 }, &dwBytesWritten);
		++t;
		for (int i = 0; i < 6; ++i) {
			wasdUD[i] = false;
		}
	}
}