#include <iostream>
#include <string>
#include <windows.h>
#include "console.h"

// В конструкторе получаем дескриптор консоли и текущее значение цвета
ConsoleColor::ConsoleColor()
{
	// Текущее значение цвета, если будет ошибка при определении, ставим принудительно
	oldColor = GRAY;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (GetConsoleScreenBufferInfo(hConsole, &infoBuff))
		oldColor = infoBuff.wAttributes;	
}

ConsoleColor::~ConsoleColor()
{
	restore();
}

// Ставим цвет консоли, если не указан, то дефолтное значение
void ConsoleColor::set_color(WORD color)
{
	SetConsoleTextAttribute(hConsole, color);	
}

// Восстановить старое значение цвета текста в консоли
void ConsoleColor::restore()
{
	SetConsoleTextAttribute(hConsole, oldColor);
}
