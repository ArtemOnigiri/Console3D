#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include "VecFunctions.h"

int width = 120 * 2, height = 30 * 2;

#if defined(_WIN32) || defined(_WIN64)

// Windows
#include <windows.h>
#include <conio.h>

int findWindowSize() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

#else

// *nix
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

void findWindowSize() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	height = w.ws_row - 1;
	width = w.ws_col;
}
#endif

int main() {

	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	findWindowSize();
	
	float aspect = (float)width / height;
	float pixelAspect = 11.0f / 24.0f;
	std::string gradient = " .:!/r(l1Z4H9W8$@";
	int gradientSize = gradient.size() - 1;

	std::string str(width, ' ');
	std::vector <std::string> screen(height, str);
	
	for (int t = 0; t < 10000; t++) {
		vec3 light = norm(vec3(-0.5, 0.5, -1.0));
		vec3 spherePos = vec3(0, 3, 0);
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				vec2 uv = vec2(i, j) / vec2(width, height) * 2.0f - 1.0f;
				uv.x *= aspect * pixelAspect;
				vec3 ro = vec3(-6, 0, 0);
				vec3 rd = norm(vec3(2, uv));
				ro = rotateY(ro, 0.25);
				rd = rotateY(rd, 0.25);
				ro = rotateZ(ro, t * 0.01);
				rd = rotateZ(rd, t * 0.01);
				float diff = 1;
				for (int k = 0; k < 5; k++) {
					float minIt = 99999;
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
					if (minIt < 99999) {
						diff *= (dot(n, light) * 0.5 + 0.5) * albedo;
						ro = ro + rd * (minIt - 0.01);
						rd = reflect(rd, n);
					}
					else break;
				}
				int color = (int)(diff * 20);
				color = clamp(color, 0, gradientSize);
				char pixel = gradient[color];
				screen[j][i] = pixel;
			}
		}

		for (int i = 0; i < height; i++) {
			std::cout << screen[i] << "\n";
		}

		#if defined(_WIN32) || defined(_WIN64)
			gotoxy(1,1);
		#else
			std::cout << "\033[0;0f";
		#endif
		
		std::cout.flush();
	}
}

