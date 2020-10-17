#include <iostream>
#include <windows.h>

#pragma once

enum Colors {
	BLUE = 1,
	GREEN,
	AQUA,
	RED,
	PURPLE,
	YELLOW,
	WHITE,
	GRAY,
	LIGHT_BLUE,
	LIGHT_GREEN,
	LIGHT_AQUA,
	LIGHT_RED,
	LIGHT_PURPLE,
	LIGHT_YELLOW,
	BRIGHT_WHITE,
};

class ConsoleColor
{
public:
	ConsoleColor();
	~ConsoleColor();
	void set_color(WORD color);
	void restore();
private:
	HANDLE hConsole;
	CONSOLE_SCREEN_BUFFER_INFO infoBuff;
	WORD oldColor;
};

