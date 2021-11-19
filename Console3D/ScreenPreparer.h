#pragma once
#include <iostream>

const char CHARS_GRADIENT[] = " .:!/r(l1Z4H9W8$@";
const size_t COLORS_COUNT = std::size(CHARS_GRADIENT) - 2;
const size_t WINDOW_WIDTH = 120 * 2;
const size_t WINDOW_HEIGHT = 30 * 2;
const float WINDOW_ASPECT = static_cast<float>(WINDOW_WIDTH / WINDOW_HEIGHT);
const float CONSOLE_CHAR_ASPECT = 11.0f / 24.0f;
const size_t ANIMATION_TIME = 10000;

void PrepareWindow(size_t width, size_t height);
void RedrawScreenBuffer(HANDLE hConsole, wchar_t* &screenBuffer);
HANDLE GetConsoleHandle();
wchar_t* CreateScreenBuffer();