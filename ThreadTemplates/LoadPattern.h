#pragma once

#include <stdio.h>
#include <string.h>

#include "PlayPattern.h"

int loadPattern()
{
	FILE* ft;
	errno_t err;

	int pattern[5][2];
	char filename[20];

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

	for(int i = 0; i < 5; ++i)
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