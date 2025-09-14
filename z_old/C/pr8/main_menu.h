#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

void show_menu(int mt, int mcl)
{
    if(mt == 0) 
    {
        if(mcl == 0)
            printf("|> New game |\n| Exit      |");
        else if(mcl == 1)
            printf("| New game  |\n|> Exit     |");
    } else if(mt == 1)
    {
        if(mcl == 0)
            printf("|> Continue |\n| New game  |\n| Exit      |");
        else if(mcl == 1)
            printf("| Continue  |\n|> New game |\n| Exit      |");
        else if(mcl == 2)
            printf("| Continue  |\n| New game  |\n|> Exit     |");
    }
}

int move_in_menu()
{
    int menu_type;
    int menu_cursor_location = 0;
    char keyboard_input;
    char c = '\0';
    FILE* f = fopen("menu_type.game", "rb");
    fread(&menu_type, sizeof(int), 1, f);
    fclose(f);

    show_menu(menu_type, menu_cursor_location);

    if(menu_type == 0) {

        while (1)
        {
            if(kbhit())
                c = getch();
                
            if(c == 'w') {
                c = '\0';

                if(menu_cursor_location == 0) 
                    continue;
                
                menu_cursor_location--;
                system("cls");
                show_menu(menu_type, menu_cursor_location);
            } else if(c == 's') {
                c = '\0';

                if(menu_cursor_location == 1) 
                    continue;
                
                menu_cursor_location++;
                system("cls");
                show_menu(menu_type, menu_cursor_location);
            } else if(c == 'e') {
                if(menu_cursor_location == 0) {
                    return 1;
                } else if(menu_cursor_location == 1) {
                    return 0;
                }
            }
        }
    } else if(menu_type == 1) {

        while (1)
        {
            if(kbhit())
                c = getch();
                
            if(c == 'w') {
                c = '\0';

                if(menu_cursor_location == 0) 
                    continue;
                
                menu_cursor_location--;
                system("cls");
                show_menu(menu_type, menu_cursor_location);
            } else if(c == 's') {
                c = '\0';

                if(menu_cursor_location == 2) 
                    continue;
                
                menu_cursor_location++;
                system("cls");
                show_menu(menu_type, menu_cursor_location);
            } else if(c == 'f') {
                if(menu_cursor_location == 0) {
                    return 2;
                } else if(menu_cursor_location == 1) {
                    return 1;
                } else if(menu_cursor_location == 2) {
                    return 0;
                }
            }
        }
    }
}