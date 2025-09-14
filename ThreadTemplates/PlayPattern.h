#pragma once

#include <stdio.h>
#include <Windows.h>

DWORD WINAPI threadFunction(LPVOID lpParam) {
	// �������� ��������� ������
	int* params = (int*)lpParam;
	int threadNumber = params[0];
	int duration = params[1];
	// ��������� � ������ ������ ������
	printf("%d-� ����� ����� ������\n", threadNumber);
	// ��������� ������ ������ (������������)
	Sleep(duration * 1000); // Sleep ��������� ������������, ������� �������� �� 1000
	// ��������� � ���������� ������ ������
	printf("%d-� ����� �������� ������\n", threadNumber);
	return 0;
}

int playPattern(int params[5][2])
{
	HANDLE threads[5];
	for (int i = 0; i < 5; i++) {
		threads[i] = CreateThread(NULL, 0, threadFunction, &params[i], 0,
			NULL);
		if (threads[i] == NULL) {
			printf("������ �������� ������ %d\n", i + 1);
			return 1;
		}
	}

	// �������� ���������� ���� �������
	WaitForMultipleObjects(5, threads, TRUE, INFINITE);
	// �������� ������������ �������
	for (int i = 0; i < 5; i++) {
		CloseHandle(threads[i]);
	}

	printf("\n������ ��������� �������..\n");

	return 0;
}