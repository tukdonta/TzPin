/*
 *	ТЗ. Разработчик. Подбор PIN по хешу.
 *		- Есть MD5 хеш от пинкода из 8 цифр ‘0'…’9' (ASCII символов).
 *		- На вход программы дается этот MD5 хеш.
 *		- Программа должна перебором определить пинкод, от которого взят данный хеш.
*/

#include <iostream>
#include <string>
#include <cmath>
#include <future>
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include "lib.h"
#include "hash.h"
#include "console.h"

// Функция для подбора и сравнения MD5 
int task_brute(char* md5hash, int begin_numbers, int end_numbers, std::atomic<int>* result);

int main(int argc, char* argv[], char* envp[])
{	
	// Код выхода программы
	int status = NORMAL;

	// Кол-во потоков для анализа
	int count_threads = std::thread::hardware_concurrency();

	std::atomic<int> pin_result(PIN_NOT_FOUND);

	// Управляем цветом в консоли
	ConsoleColor console;
	
	// Режим отображения для кириллицы
	int mode = _setmode(_fileno(stdout), _O_U8TEXT);

	if (argc == 1) {
		console.set_color(LIGHT_YELLOW);
		init_message();
		status = WITHOUT_ARG;
	}

	if ((argc > 1) && (strlen(argv[1]) != MD5_LEN)) {
		console.set_color(RED);
		std::wcout << L"Длина хэша не равна 32 символам: " << argv[1] << std::endl;		
		status = ERR_MD5LEN;
	}		

	if ((status == NORMAL) && (!check_input_md5(argv[1]))) {
		console.set_color(RED);
		std::wcout << L"Хэш содержит недопустимые символы: " << argv[1] << std::endl;
		status = ERR_MD5_WRONG_CHAR;
	}

	// Используются цифры от 0..9 (возможны повторения вида 88881111)
	// (кол-во комбинаций 10 в степени PIN_LEN) - 1
	#pragma warning(disable: 4244 ) 
	int combinations = std::pow(10, PIN_LEN) - 1;
	#pragma warning(default: 4244)

	// Проверки прошли, основная работа
	if (status == NORMAL) {
		console.set_color(GREEN);
		std::wcout << L"Поиск пина ..." << std::endl;
		std::wcout << L"Будет запущено потоков: " << count_threads << std::endl;

		// Количество пинов для каждой задачи
		int slice = combinations / count_threads;
		// Список задач
		std::vector<std::future<int>> tasks;

		// Готовим данные для задач
		for (int t = 0; t < count_threads; t++) {
			int begin_num = t * slice;
			int end_num = begin_num + slice - 1;
			if ((t + 1) == count_threads)
				end_num += count_threads;

			std::wcout << L"Поток (" << t << L") диапазон с " << begin_num << L" по " << end_num << std::endl;

			tasks.push_back( std::async(std::launch::async, task_brute, argv[1], begin_num, end_num, &pin_result) );
		}			

		// Запуск задач
		for (std::future<int>& task : tasks)
			task.get();	

		// Отображаем результат работы
		if (pin_result == PIN_NOT_FOUND) {
			console.set_color(LIGHT_PURPLE);
			std::wcout << L"Пин не был обнаружен для заданного хэша" << std::endl;
		}
		else {
			console.set_color(BRIGHT_WHITE);
			std::wcout << L"  >>> Пин найден: ";
			std::wcout.width(PIN_LEN);
			std::wcout.fill('0');
			std::wcout << pin_result << std::endl;
		}

		console.set_color(GREEN);
		std::wcout << L"Поиск закончен." << std::endl;

		// Вернуть код выхода при завершении программы
		status = (pin_result == PIN_NOT_FOUND ? PIN_NOT_FOUND : PIN_FOUND);
		 
	}	

	return status;
}

int task_brute(char* md5hash, int begin_numbers, int end_numbers, std::atomic<int>* result)
{
	int r = PIN_NOT_FOUND;	

	// Объект для получения MD5 из числа (пина)
	Hash h;	

	// Расчитанный MD5 в виде строки
	char* md5brute;	

	for (int i = begin_numbers; i <= end_numbers; i++) {
		md5brute = h.GetMD5(i);
		if (md5brute) {
			if (!_strcmpi(md5hash, md5brute))
				r = i;

			free(md5brute);

			if (r != PIN_NOT_FOUND) {
				result->store(r);
				break;
			}
		}

		if (result->load() != PIN_NOT_FOUND) {
			break;
		}
	}

	return r;
}