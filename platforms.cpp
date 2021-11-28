#include <iostream>

int width = 120 * 2, height = 30 * 2;

#if defined(_WIN32) || defined(_WIN64)

// Windows
#include <windows.h>
int findWindowSize() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	width = csbi.srWindow.Right - csbi.srWindow.Left;
	height = csbi.srWindow.Bottom - csbi.srWindow.Top;
}

void gotoxy(int x, int y) {
	COORD p = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

void clearScreen() {
	system("cls");
}

#else

// *nix
#include <sys/ioctl.h>
#include <unistd.h>

void findWindowSize() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	height = w.ws_row - 1;
	width = w.ws_col;
}

void gotoxy(int x, int y) {
	x++; y++;
	std::cout << "\033[" << y << ";" << x << "f";
}

void clearScreen() {
	std::cout << "\033[2J";
	gotoxy(0,0);
}

#endif
