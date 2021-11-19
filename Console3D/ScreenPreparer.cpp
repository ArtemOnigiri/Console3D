#include <windows.h>
#include <stdio.h>
#include "ScreenPreparer.h"

void PrepareWindow(size_t width, size_t height) {
	_COORD coord;
	coord.X = static_cast<SHORT>(width);
	coord.Y = static_cast<SHORT>(height);
	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = static_cast<SHORT>(height) - 1;
	Rect.Right = static_cast<SHORT>(width) - 1;
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(Handle, coord);
	SetConsoleWindowInfo(Handle, TRUE, &Rect);
}

HANDLE GetConsoleHandle() {
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);

	return hConsole;
}

wchar_t* CreateScreenBuffer() {
	return new wchar_t[WINDOW_WIDTH * WINDOW_HEIGHT];
}

void RedrawScreenBuffer(HANDLE hConsole, wchar_t* &screenBuffer) {
	DWORD dwBytesWritten = 0;
	screenBuffer[WINDOW_WIDTH * WINDOW_HEIGHT - 1] = '\0';
	WriteConsoleOutputCharacter(hConsole, screenBuffer, WINDOW_WIDTH * WINDOW_HEIGHT, { 0, 0 }, &dwBytesWritten);
}