#include <stdio.h>
#include <locale.h>

#include "PatternFunctions.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    printf("������..\n");

    char choice;

    while (true) {
        printf("\n�������� ��������:\nn - ������� ����� ������ �������\nl - ��������� ������������ ������ �������\nx - �����\n> ");

        scanf_s(" %c", &choice);

        if (choice == 'x') { break; }
        else  if (choice == 'l') {
            loadPattern();
        }
        else if (choice == 'n') {
            int result = newPattern();
        }
        else {
            printf("\n����������� ��������! ���������� ��� ���..\n\n");
        }
    }

    printf("\n����������..\n");
    return 0;
}
