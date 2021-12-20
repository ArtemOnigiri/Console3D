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

vec3 light = norm(vec3(0.0, 0.0f, -1.0f));
int frames = 0;
std::vector<std::vector<vec3>> pyramid(3 * 2);
std::vector<std::vector<vec3>> prism(5 * 4);

vec3 sph1Pos = vec3(-3, 0.5, 0.2);
vec3 sph2Pos = vec3(0);
vec2 box1Pos = vec2(-2.6, -0.9);
vec2 box2Pos = vec2(-0.4, -0.9);
vec3 pyramidPos = vec3(-1.5, -2.5, 1);
vec3 prismPos = vec3(1.5, 2.5, 1);

double GetSky(vec3 rd) {
	double col = 90.0;
	double sun = 1.0;
	sun *= max(0.0, pow(dot(rd, light), 128.0));
	col *= max(0.0, dot(light, vec3(0.0, 0.0, -1.0)));
	return clamp(sun + col * 0.01, 0.0, 1.0);
}

vec2 rotation(double r, double t, double d = 0.0)
{
	double time = (frames - d) - (t * floor((frames - d) / t));
	if (frames < d) time = 0;
	double angle = 360 * (time / t) * (3.1415926 / 180);
	double x = r * cos(angle);
	double y = r * sin(angle);
	return vec2(x, y);
}

vec2 CastRay(vec3 &ro, vec3 &rd) {
	double minIt = 9999;
	vec2 col = vec2(0.0);
	vec3 n = 0;

	vec2 intersection = sphere(ro - sph1Pos, rd, 0.8);
	if (intersection.x > 0) {
		vec3 itPoint = ro - sph1Pos + rd * intersection.x;
		minIt = intersection.x;
		n = norm(itPoint);
		col = vec2(1.0, -0.5);
	}

	intersection = sphere(ro - sph2Pos, rd, 1);
	if (intersection.x > 0 && intersection.x < minIt) {
		vec3 itPoint = ro - sph2Pos + rd * intersection.x;
		minIt = intersection.x;
		n = norm(itPoint);
		col = vec2(1.0, 1.0);
	}

	vec3 boxN = 0;
	if (frames >= 25) {
		box1Pos = rotation(1.1, 50) + vec2(-1.5, -0.9);
		box2Pos = rotation(1.1, 50, 25) + vec2(-1.5, -0.9);
	}

	intersection = box(ro - vec3(2.5, box1Pos.x, box1Pos.y), rd, vec3(0.9), boxN);
	if (intersection.x > 0 && intersection.x < minIt) {
		minIt = intersection.x;
		n = boxN;
		col = vec2(0.3, 1.0);
	}

	intersection = box(ro - vec3(2.5, box2Pos.x, box2Pos.y), rd, vec3(0.9), boxN);
	if (intersection.x > 0 && intersection.x < minIt) {
		minIt = intersection.x;
		n = boxN;
		col = vec2(0.3, 1.0);
	}

	intersection = sphere(ro - vec3(pyramidPos.x, pyramidPos.y, pyramidPos.z / 2), rd, 1.5);
	if (intersection.x != -1 && intersection.x < minIt) {
		for (int a = 0; a < pyramid.size(); ++a) {
			intersection = vec2(triangle(ro, rd, pyramid[a][0], pyramid[a][1], pyramid[a][2]).x, triangle(ro, rd, pyramid[a][0], pyramid[a][1], pyramid[a][2]).z);
			if (intersection.x > 0 && intersection.x < minIt) {
				minIt = intersection.x;
				vec3 v0 = pyramid[a][0] - pyramid[a][1];
				vec3 v1 = pyramid[a][2] - pyramid[a][1];
				n = norm(cross(v0, v1));
				col = vec2(0.25, 1.0);
			}
		}
	}

	intersection = sphere(ro - vec3(prismPos.x, prismPos.y, -0.75), rd, 2);
	if (intersection.x != -1 && intersection.x < minIt) {
		for (int a = 0; a < prism.size(); ++a) {
			intersection = vec2(triangle(ro, rd, prism[a][0], prism[a][1], prism[a][2]).x, triangle(ro, rd, prism[a][0], prism[a][1], prism[a][2]).z);
			if (intersection.x > 0 && intersection.x < minIt) {
				minIt = intersection.x;
				vec3 v0 = prism[a][0] - prism[a][1];
				vec3 v1 = prism[a][2] - prism[a][1];
				n = norm(cross(v0, v1));
				col = vec2(0.4, 1.0);
			}
		}
	}

	intersection = plane(ro, rd, vec3(0, 0, -1), 1);
	if (intersection.x > 0 && intersection.x < minIt) {
		minIt = intersection.x;
		n = vec3(0, 0, -1);
		col = vec2(0.35, 1.0);
	}

	if (minIt == 9999) return vec2(GetSky(rd), -2.0);

	if (col.y == -2.0) return col;
	vec3 reflected = reflect(rd, n);
	if (col.y < 0.0) {
		double fresnel = 1.0 - abs(dot(-rd, n));
		if ((double)(rand()) / ((double)(RAND_MAX)) - 0.1 < fresnel * fresnel) {
			rd = reflected;
			return col;
		}
		ro = ro + rd * (minIt + 0.001);
		rd = refract(rd, n, 1.0 / (1.0 - col.y));
		return col;
	}
	vec3 itPos = ro + rd * intersection.x;
	vec3 r = vec3(rand(), rand(), rand());
	vec3 diffuse = norm(r * dot(r, n));
	ro = ro + rd * (minIt - 0.001);
	rd = mix(diffuse, reflected, col.y);
	return col;
}

double TraceRay(vec3 ro, vec3 rd) {
	double col = 1.0;
	for (int i = 0; i < 4; i++)
	{
		vec2 refCol = CastRay(ro, rd);
		col *= refCol.x;
		if (refCol.y == -2.0) return col;
	}
	return 0.0;
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
	double speed = 0.3f;
	vec3 pos = vec3(-8, -2, -2);

	wchar_t* screen = new wchar_t[width * height];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	Pyramid(3, -2.0, 1.5, pyramidPos, pyramid);
	Prism(5, -3.0, 1.0, prismPos, prism);

	while (true) {
		POINT p;
		GetCursorPos(&p);
		double mx = p.x - w / 2;
		double my = p.y - h / 2;
		mouseX += mx;
		mouseY += my;
		SetCursorPos(w / 2, h / 2);

		if (GetAsyncKeyState(VK_ESCAPE)) break;
		if (GetAsyncKeyState(87)) wasdUD[0] = true;
		if (GetAsyncKeyState(65)) wasdUD[1] = true;
		if (GetAsyncKeyState(83)) wasdUD[2] = true;
		if (GetAsyncKeyState(68)) wasdUD[3] = true;
		if (GetAsyncKeyState(32)) wasdUD[4] = true;
		if (GetAsyncKeyState(67)) wasdUD[5] = true;

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
				double col = TraceRay(ro, rd);
				int color = (int)gradientSize * col;
				char pixel = gradient[gradientSize - color];
				screen[i + j * width] = pixel;
			}
		}
		screen[width * height - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, width * height, { 0, 0 }, &dwBytesWritten);

		for (int i = 0; i < 6; ++i) {
			wasdUD[i] = false;
		}
		++frames;
	}
}