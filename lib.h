/*
 *	Сервисные функции проекта.
*/
#pragma once

// Длина хэша
constexpr int MD5_LEN = 32;
// Длина пин-кода из хеша
constexpr int PIN_LEN = 8;
// Длина буфера для получения MD5 от крипто-провайдера
constexpr int BUFF_MD5_LEN = 16;
// Размер буфера для MD5 в строковом представлении
constexpr int BUFF_MD5_STRING = (BUFF_MD5_LEN + 1) * 2;
// Результат поиска пина
constexpr int PIN_NOT_FOUND = -1;

// Статусы программы
enum AppStatus 
{
	// Нормальное завершение
	NORMAL = 0,
	// Ошибка - хэше с неверной длиной
	ERR_MD5LEN,
	// Программа без аргументов, покажем сообщение
	WITHOUT_ARG,
	// MD5 содержит недопустимые символы
	ERR_MD5_WRONG_CHAR,
	// Пин найден
	PIN_FOUND
};


void init_message();

bool check_input_md5(const char* md5str);


