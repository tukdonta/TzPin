#include <windows.h>
#include <Wincrypt.h>
#include "lib.h"

#pragma once
class Hash
{
private:
	HCRYPTPROV hCryptProv = NULL;
	// Длина буфера для копирования строкового представления пина из числа
	int len_buff_pin = PIN_LEN + 1;
	// Пин в виде строки
	char buff_pin[PIN_LEN + 1];
	// Результат после вычисления MD5 для пина
	byte buff_result[BUFF_MD5_LEN + 1];
	// Преобразование буфера в строковый HEX вид MD5
	char hex[BUFF_MD5_LEN + 1] = "0123456789abcdef";
	// Произошла ошибка
	bool error = false;

public:
	Hash();
	~Hash();
	void dispose();
	bool IsError();
	char* GetMD5(int pin);
};

