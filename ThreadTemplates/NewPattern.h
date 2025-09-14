#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "SavePattern.h"
#include "PlayPattern.h"

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
		if (res1 == 1)
			return 1;

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