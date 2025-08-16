#include <stdio.h>
#include <conio.h>

typedef struct tag_coords COORDINATES;
typedef struct tag_item ITEM;
typedef struct tag_character CHARACTER;
typedef struct tag_object OBJECT;

void delay(int);
void game_save(CHARACTER* character, OBJECT* ar_of_objects[], int count_of_objs);

void show_in_game_menu(int mcl)
{
    if(mcl == 0)
        printf("|> Continue |\n| Save      |\n| Exit      |");
    else if(mcl == 1)
        printf("| Continue  |\n|> Save     |\n| Exit      |");
    else if(mcl == 2)
        printf("| Continue  |\n| Save      |\n|> Exit     |");
}

int move_in_in_game_menu(CHARACTER* character, OBJECT* ar_of_objects[], int count_of_objs)
{
    int menu_cursor_location = 0;
    char keyboard_input;
    char c = '\0';

    system("cls");
    show_in_game_menu(menu_cursor_location);

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
            show_in_game_menu(menu_cursor_location);
        } else if(c == 's') {
            c = '\0';

            if(menu_cursor_location == 2) 
                continue;
            
            menu_cursor_location++;
            system("cls");
            show_in_game_menu(menu_cursor_location);
        } else if(c == 'f') {
            c = '\0';
            if(menu_cursor_location == 0) {
                return 1;
            } else if(menu_cursor_location == 1) {
                game_save(character, ar_of_objects, count_of_objs);
                delay(1);
                system("cls");
                show_in_game_menu(menu_cursor_location);
            } else if(menu_cursor_location == 2) {
                return 0;
            }
        }
    }
}