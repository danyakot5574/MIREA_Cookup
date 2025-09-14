#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include "game.h"
#include "main_menu.h"

int move_in_menu();
void game_start();
void load_game();

int main(void)
{
    SetConsoleOutputCP(CP_UTF8);

    int menu_exit_code = move_in_menu();

    switch (menu_exit_code)
    {
    case 0: break;
    
    case 1:
        game_start();
        break;
    
    case 2:
        load_game();
        break;
    
    default: break;
    }
    
    return 0;
}
