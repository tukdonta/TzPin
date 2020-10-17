#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING  1

#include <iostream>
#include <regex>
#include "lib.h"

// Описание программы и параметров
void init_message()	
{
	std::wcout << L"Подбор PIN по хешу (Зубарев Андрей)" << std::endl;
	std::wcout << L"Использование: pinhash <md5hash>" << std::endl;
	std::wcout << L"  md5hash - хэш для подбора пина" << std::endl;	
}

// Проверяет валидность входного хэша : true - все хорошо
bool check_input_md5(const char* md5str)
{
	std::string str = md5str;
	std::tr1::regex rx("[A-Za-z0-9]+");

	return regex_match(str.begin(), str.end(), rx);
}