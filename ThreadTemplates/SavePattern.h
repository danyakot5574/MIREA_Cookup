#pragma once

#include <stdio.h>

int savePattern(int pattern[5][2], const char * filename)
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