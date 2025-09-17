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

int savePattern(int pattern[5][2], const char* filename)
{
	FILE* ft;

	errno_t err;

	if ((err = fopen_s(&ft, filename, "w")) != 0)
	{
		printf("�� �������� ������� ���� �� ������! ����� � ������� ����..");
		return 1;
	}

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 2; j++)
		{
			fwrite(&pattern[i][j], sizeof(pattern[i][j]), 1, ft);
		}

	printf("\n������ ��� ������� ������� � ���� %s! ����� � ������� ����..\n", filename);
	return 0;
}

int newPattern()
{
	while (1) {
		int params[5][2];

		for (int i = 0; i < 5; ++i)
		{
			int duration;
			printf("������� ����������������� %d-�� ������: ", i + 1);

			scanf_s("%d", &duration);

			params[i][0] = i + 1;
			params[i][1] = duration;
		}

		int res1 = playPattern(params);
		if (res1 == 1) {
			printf("\n� ���� �������� ������ ������� ��������� ����������� ������, ���������� ��� ���..\n");
			return 1;
		}

		char choice;

		while (1) {
			printf("�������� ��������:\nr - ������� ����� ������\ns - ��������� ������� ������\nx - ����� � ������� ����\n> ");

			scanf_s(" %c", &choice);

			if (choice == 'r') { break; }
			else if (choice == 's') {
				char filename[20];

				printf("\n������� �������� ����� (���� - 20 ��������): ");

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
				printf("\n����������� ��������! ���������� ��� ���..\n");
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

	printf("\n������� �������� ����� (���� - 20 ��������): ");

	fgets(filename, sizeof(filename) - 1, stdin);
	fgets(filename, sizeof(filename) - 1, stdin);
	char* ptr_n = strrchr(filename, '\n');
	if (ptr_n != NULL)
		*ptr_n = '\0';

	printf("%s", filename);

	if (err = fopen_s(&ft, filename, "r") != 0)
	{
		printf("\n�� ������� ������� ���� �� ������! ����� � ������� ����..\n");
		return 1;
	}

	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 2; ++j)
		{
			fread(&pattern[i][j], sizeof(pattern[i][j]), 1, ft);
		}

	printf("\n�����\t������������\n");
	for (int i = 0; i < 5; ++i)
		printf("\n%d\t%d", pattern[i][0], pattern[i][1]);

	char choice;
	while (true) {
		printf("\n�������� ��������:\np - ��������� ������\nx - ����� � ������� ����\n");
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
			printf("\n����������� ��������! ���������� ��� ���..\n");
		}
	}

	return 0;
}