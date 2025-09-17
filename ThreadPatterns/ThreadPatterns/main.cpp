#include <stdio.h>
#include <locale.h>

#include "PatternFunctions.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    printf("Запуск..\n");

    char choice;

    while (true) {
        printf("\nВыберите действие:\nn - создать новый шаблон потоков\nl - загрузить существующий шаблон потоков\nx - выйти\n> ");

        scanf_s(" %c", &choice);

        if (choice == 'x') { break; }
        else  if (choice == 'l') {
            loadPattern();
        }
        else if (choice == 'n') {
            int result = newPattern();
        }
        else {
            printf("\nНеизвестный параметр! Попробуйте ещё раз..\n\n");
        }
    }

    printf("\nЗавершение..\n");
    return 0;
}
