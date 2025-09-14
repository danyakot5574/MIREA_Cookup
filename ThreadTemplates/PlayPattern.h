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