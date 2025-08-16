#include <stdio.h>
#include <stdlib.h>

int main()
{
    int menu_type = 0;
    FILE* f = fopen("menu_type.game", "wb");
    fwrite(&menu_type, sizeof(int), 1, f);
    fclose(f);

    return 0;
}