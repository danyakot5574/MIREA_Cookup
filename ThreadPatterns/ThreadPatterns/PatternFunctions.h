#pragma once

#include <stdio.h>
#include <Windows.h>

DWORD WINAPI threadFunction(LPVOID lpParam) {
	// Получаем параметры потока
	int* params = (int*)lpParam;
	int threadNumber = params[0];
	int duration = params[1];
	// Сообщение о начале работы потока
	printf("%d-й поток начал работу\n", threadNumber);
	// Симуляция работы потока (приостановка)
	Sleep(duration * 1000); // Sleep принимает миллисекунды, поэтому умножаем на 1000
	// Сообщение о завершении работы потока
	printf("%d-й поток выполнил задачу\n", threadNumber);
	return 0;
}

int playPattern(int params[5][2])
{
	HANDLE threads[5];
	for (int i = 0; i < 5; i++) {
		threads[i] = CreateThread(NULL, 0, threadFunction, &params[i], 0,
			NULL);
		if (threads[i] == NULL) {
			printf("Ошибка создания потока %d\n", i + 1);
			return 1;
		}
	}

	// Ожидание завершения всех потоков
	WaitForMultipleObjects(5, threads, TRUE, INFINITE);
	// Закрытие дескрипторов потоков
	for (int i = 0; i < 5; i++) {
		CloseHandle(threads[i]);
	}

	printf("\nШаблон отработал успешно..\n");

	return 0;
}

int savePattern(int pattern[5][2], const char* filename)
{
	FILE* ft;

	errno_t err;

	if ((err = fopen_s(&ft, filename, "w")) != 0)
	{
		printf("Не удалость открыть файл на запись! Выход в главное меню..");
		return 1;
	}

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 2; j++)
		{
			fwrite(&pattern[i][j], sizeof(pattern[i][j]), 1, ft);
		}

	printf("\nШаблон был успешно сохранён в файл %s! Выход в главное меню..\n", filename);
	return 0;
}

int newPattern()
{
	while (1) {
		int params[5][2];

		for (int i = 0; i < 5; ++i)
		{
			int duration;
			printf("Введите продолжительность %d-го потока: ", i + 1);

			scanf_s("%d", &duration);

			params[i][0] = i + 1;
			params[i][1] = duration;
		}

		int res1 = playPattern(params);
		if (res1 == 1) {
			printf("\nВ ходе создания нового шаблона произошла неизвестная ошибка, попробуйте ещё раз..\n");
			return 1;
		}

		char choice;

		while (1) {
			printf("Выберите действие:\nr - создать новый шаблон\ns - сохранить текущий шаблон\nx - выйти в главное меню\n> ");

			scanf_s(" %c", &choice);

			if (choice == 'r') { break; }
			else if (choice == 's') {
				char filename[20];

				printf("\nВведите название файла (макс - 20 символов): ");

				fgets(filename, sizeof(filename) - 1, stdin);
				fgets(filename, sizeof(filename) - 1, stdin);
				char* ptr_n = strrchr(filename, '\n');
				if (ptr_n != NULL)
					*ptr_n = '\0';

				int result = savePattern(params, filename);

				return 0;
			}
			else if (choice == 'x')
			{
				return 0;
			}
			else {
				printf("\nНеизвестный параметр! Попробуйте ещё раз..\n");
			}
		}
	}

	return 0;
}

int loadPattern()
{
	FILE* ft;
	errno_t err;

	int pattern[5][2];
	char filename[20];

	printf("\nВведите название файла (макс - 20 символов): ");

	fgets(filename, sizeof(filename) - 1, stdin);
	fgets(filename, sizeof(filename) - 1, stdin);
	char* ptr_n = strrchr(filename, '\n');
	if (ptr_n != NULL)
		*ptr_n = '\0';

	printf("%s", filename);

	if (err = fopen_s(&ft, filename, "r") != 0)
	{
		printf("\nНе удалось открыть файл на чтение! Выход в главное меню..\n");
		return 1;
	}

	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 2; ++j)
		{
			fread(&pattern[i][j], sizeof(pattern[i][j]), 1, ft);
		}

	printf("\nНомер\tДлительность\n");
	for (int i = 0; i < 5; ++i)
		printf("\n%d\t%d", pattern[i][0], pattern[i][1]);

	char choice;
	while (true) {
		printf("\nВыберите действие:\np - запустить шаблон\nx - выйти в главное меню\n");
		scanf_s(" %c", &choice);

		if (choice == 'p')
		{
			playPattern(pattern);
			break;
		}
		else if (choice == 'x')
		{
			break;
		}
		else {
			printf("\nНеизвестный параметр! Попробуйте ещё раз..\n");
		}
	}

	return 0;
}