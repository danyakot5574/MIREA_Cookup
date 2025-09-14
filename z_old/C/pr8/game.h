#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include "in_game_menu.h"

int move_in_in_game_menu(CHARACTER* character, OBJECT* ar_of_objects[], int count_of_objs);

typedef struct tag_coords
{
    int x, y;
} COORDINATES;

typedef struct tag_item
{
    char name[15];
    int name_len;
    int id;
}ITEM;

typedef struct tag_character
{
    COORDINATES crd;
    ITEM* inventory[5];
    int count_of_items;
} CHARACTER;

typedef struct tag_object
{
    char name[15];
    char action[15];
    char use_msg[25];
    char cant_use_msg[25];
    bool is_used;
    COORDINATES interact_coordinates[4];
} OBJECT;

void delay(int number_of_seconds)
{
    int milli_seconds = 1000 * number_of_seconds;

    clock_t start_time = clock();

    while (clock() < start_time + milli_seconds);
}

int check_coordinates(COORDINATES* ar, int len, COORDINATES arg)
{
    for(int i = 0; i < len; i++)
        if(ar[i].x == arg.x && ar[i].y == arg.y)
            return 1;
    return 0;
}

int check_obj_coordinates(OBJECT* ar_of_objects[], int len, COORDINATES arg)
{
    for(int i = 0; i < len; i++)
        for(int j = 0; j < 4; j++)
            if(ar_of_objects[i]->interact_coordinates[j].x == arg.x && ar_of_objects[i]->interact_coordinates[j].y == arg.y)
                return i;
    
    return -1;
}

int check_inventory(CHARACTER* character, char item_id)
{
    for(int i = 0; i < character->count_of_items; i++) 
        if(character->inventory[i]->id == item_id)
            return 1;
    return 0;
}

void add_item(CHARACTER* character, char* name, int name_len, int id)
{
    ITEM* new_item = malloc(sizeof(ITEM));
    if (new_item == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    strcpy(new_item->name, name);
    new_item->name_len = name_len;
    new_item->id = id;

    character->inventory[character->count_of_items] = new_item;
    character->count_of_items++;
}

void output_map(char* ui[], char* map[], CHARACTER person)
{
    for (int i = 0; i < 10; i++) 
            strncpy(ui[i], map[i], 22);
    
    ui[person.crd.y][person.crd.x] = 'i';

    return;
}

void output_hints(char* ui[], COORDINATES crd, bool* output_condition)
{
    COORDINATES w_block[24] = {{1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, {8, 1}, {9, 1},
                                {10, 1}, {11, 1}, {12, 1}, {13, 1}, {14, 1}, {15, 1}, {16, 1}, {17, 1},
                                {18, 1}, {13, 5}, {14, 5}, {15, 5}, {17, 5}, {18, 5}, {18, 2}};
    COORDINATES a_block[12] = {{1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {14, 1},
                                {14, 2}, {14, 3}, {16, 4}};
    COORDINATES s_block[22] = {{1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 8}, {7, 8}, {8, 8}, {9, 8},
                                {10, 8}, {11, 8}, {12, 8}, {13, 8}, {14, 8}, {15, 8}, {16, 8}, {17, 8},
                                {18, 8}, {14, 3}, {15, 3}, {17, 3}, {18, 3}};
    COORDINATES d_block[14] = {{18, 1}, {18, 2}, {18, 3}, {18, 4}, {18, 5}, {18, 6}, {18, 7}, {18, 8},
                                {12, 1}, {12, 2}, {12, 3}, {12, 4}, {17, 1}, {16, 4}};
    COORDINATES obj_interact[3] = {{18, 2}, {17, 1}, {9, 8}};

    char str[9] = "    ch   ";

    if(output_condition[0] && !output_condition[3] && !output_condition[6])
    {
        if (check_coordinates(w_block, 24, crd) != 1) str[0] = 'w';
        if (check_coordinates(a_block, 12, crd) != 1) str[1] = 'a';
        if (check_coordinates(s_block, 22, crd) != 1) str[2] = 's';
        if (check_coordinates(d_block, 14, crd) != 1) str[3] = 'd';
    }

    if(!output_condition[6]) str[6] = 'e';

    if(output_condition[3])
    {
        if (output_condition[4]) str[0] = 'w';
        if (output_condition[5]) str[2] = 's';
    }

    if(output_condition[6])
    {
        if (output_condition[7]) str[1] = 'a';
        if (output_condition[8]) str[3] = 'd';
    }

    if (check_coordinates(obj_interact, 3, crd) == 1) str[7] = 'f';

    strncpy(ui[10], str, 8); 
    return;
}

void output_coordinates(char* ui[], COORDINATES arg)
{
    char op[12]; 
    sprintf(op, "x: %d; y: %d\0", arg.x, arg.y);
    strncpy(ui[11], op, 12);
    return;
}

void output_inventory(char* ui[], CHARACTER person, int icl)
{
    const char *initial_inventory[9] = {
        "╔=================╗\0", 
        "║    INVENTORY    ║\0", 
        "╠=================╣\0", 
        "║                 ║\0",
        "║                 ║\0", 
        "║                 ║\0", 
        "║                 ║\0", 
        "║                 ║\0",
        "╚=================╝\0"
    };

    int written_items = 0;

    for(int i = 0; i < 3; i++)
        strncat(ui[i], initial_inventory[i], 25);
    
    for(int i = 0; i < 6; i++)
    {
        if(written_items < person.count_of_items)
        {
            char* str = malloc(25);
            if(i == icl)
                sprintf(str, "║>  %-14s║\0", person.inventory[i]->name);
            else
                sprintf(str, "║%d) %-14s║\0", i + 1, person.inventory[i]->name);
            
            strncat(ui[i + 3], str, 25);
            written_items++;
            free(str);
        } else{
            strncat(ui[i + 3], initial_inventory[i + 3], 25);
        }
    }

    return;
}

void output_obj_menu(char* ui[], CHARACTER character, int mcl, OBJECT obj)
{
    char* msg = malloc(27);

    if(mcl == 0){
        sprintf(msg, "You see the %s. %s it?\0", obj.name, obj.action);
        strncpy(ui[13], msg, 27);
        strncpy(ui[14], ">Yes | No\0", 10);
    } else if(mcl == 1){
        sprintf(msg, "You see the %s. %s it?\0", obj.name, obj.action);
        strncpy(ui[13], msg, 27);
        strncpy(ui[14], " Yes | No<\0", 11);
    } else if(mcl == 2)
    {
        strncpy(ui[13], obj.use_msg, 27);
    } else if(mcl == 4)
    {
        strncpy(ui[13], obj.cant_use_msg, 25);
    } else if(mcl == 3)
    {
        sprintf(msg, "You already used %s!\0", obj.name);
        strncpy(ui[13], msg, 27);
    }

    free(msg);

    return;
}

void show_ui(char* map[], CHARACTER person, bool* output_condition, int icl, OBJECT* obj, int mcl)
{
    system("cls");
    char *UI[16];
    for (int i = 0; i < 16; i++){
        UI[i] = calloc(64, sizeof(char));
        if (UI[i] == NULL) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
    }

    if(output_condition[0])
        output_map(UI, map, person);
    
    if(output_condition[1])
        output_hints(UI, person.crd, output_condition);

    if(output_condition[2])
        output_coordinates(UI, person.crd);

    if(output_condition[3])
        output_inventory(UI, person, icl);
    
    if(output_condition[6])
        output_obj_menu(UI, person, mcl, *obj);


    for(int i = 0; i < 16; i++)
    {
        printf("%s\n", UI[i]);
    }

    for (int i = 0; i < 16; i++) {
        if (UI[i] != NULL)
            free(UI[i]);
    }
    
    return;
}

int move_on_obj_menu(char* map[], CHARACTER* character, bool* output_condition, OBJECT* obj)
{
    char c;
    char* str;
    int return_code = 0;
    int menu_cursor_location = 0;

    output_condition[8] = true;
    show_ui(map, *character, output_condition, 0, obj, menu_cursor_location);

    while(1)
    {
        if(kbhit()) {
            c = getch();
        }

        if(c == 'd')
        {
            c = '\0';
            if(menu_cursor_location == 0)
            {
                output_condition[8] = false;
                output_condition[7] = true;
                menu_cursor_location++;

                show_ui(map, *character, output_condition, 0, obj, menu_cursor_location);
            }
            continue;
        } else if(c == 'a')
        {
            c = '\0';
            if(menu_cursor_location == 1)
            {
                output_condition[8] = true;
                output_condition[7] = false;
                menu_cursor_location--;

                show_ui(map, *character, output_condition, 0, obj, menu_cursor_location);
            }
            continue;
        } else if(c == 'f')
        {
            c = '\0';

            if(menu_cursor_location == 0)
            {
                output_condition[8] = false;
                output_condition[7] = false;

                if(!obj->is_used)
                {
                    obj->is_used = true;
                    menu_cursor_location = 2;

                    if(strncmp(obj->name, "chest\0", 7) == 0)
                        add_item(character, "Key\0", 7, 1);
                    else if(strncmp(obj->name, "exit\0", 6) == 0) {
                        if(check_inventory(character, 1) == 0) {
                            menu_cursor_location = 4;
                            obj->is_used = false;
                        } else return_code = 3;
                    }
                    
                    show_ui(map, *character, output_condition, 0, obj, menu_cursor_location);
                    output_condition[6] = false;
                    delay(2);
                    show_ui(map, *character, output_condition, 0, obj, menu_cursor_location);
                    return return_code;
                } else
                {
                    menu_cursor_location = 3;
                    show_ui(map, *character, output_condition, 0, obj, menu_cursor_location);
                    output_condition[6] = false;
                    delay(2);
                    show_ui(map, *character, output_condition, 0, obj, menu_cursor_location);
                    return return_code;
                }
            } else
            {
                output_condition[8] = false;
                output_condition[7] = false;
                output_condition[6] = false;
                show_ui(map, *character, output_condition, 0, NULL, 0);
                return return_code;
            }

            continue;
        } else if(c == 'h')
        {
            c = '\0';
            if(output_condition[1])
                output_condition[1] = false;
            else output_condition[1] = true;

            show_ui(map, *character, output_condition, 0, obj, menu_cursor_location);
            continue;
        } else if(c == 'c')
        {
            c = '\0';
            if(output_condition[2])
                output_condition[2] = false;
            else output_condition[2] = true;

            show_ui(map, *character, output_condition, 0, obj, menu_cursor_location);

            continue;
        }
    }

    return return_code;
}

void move_on_inventory(char* map[], CHARACTER* character, bool* output_condition)
{
    char c;
    int icl = 0;

    output_condition[4] = false;

    if(character->count_of_items < 2)
        output_condition[5] = false;

    show_ui(map, *character, output_condition, icl, NULL, 0);

    while(1)
    {
        if (kbhit()) { 
            c = getch(); 
        }

        if(c == 'w')
        {
            c = '\0';
            if(icl > 0)
            {
                icl--;

                if(icl == 0)
                    output_condition[4] = false;
                
                if(!output_condition[5])
                    output_condition[5] = true;

                show_ui(map, *character, output_condition, icl, NULL, 0);
            }
            continue;
        } else if(c == 's') 
        {
            c = '\0';
            if(icl < character->count_of_items - 1)
            {
                icl++;

                if(icl == character->count_of_items - 1)
                    output_condition[5] = false;

                if(!output_condition[4])
                    output_condition[4] = true;

                show_ui(map, *character, output_condition, icl, NULL, 0);
            }
            continue;
        } else if(c == 'h')
        {
            c = '\0';
            if(output_condition[1])
                output_condition[1] = false;
            else output_condition[1] = true;

            show_ui(map, *character, output_condition, icl, NULL, 0);
        } else if(c == 'c')
        {
            c = '\0';
            if(output_condition[2])
                output_condition[2] = false;
            else output_condition[2] = true;

            show_ui(map, *character, output_condition, icl, NULL, 0);

            continue;
        } else if(c == 'e')
        {
            c = '\0';
            output_condition[3] = false;
            output_condition[4] = true;
            output_condition[5] = true;
            return;
        }
    }
}

void move_on_map(char* map[], CHARACTER* character, bool* output_condition, OBJECT* ar_of_obj[])
{
    char c;

    COORDINATES w_block[24] = {{1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, {8, 1}, {9, 1},
                                {10, 1}, {11, 1}, {12, 1}, {13, 1}, {14, 1}, {15, 1}, {16, 1}, {17, 1},
                                {18, 1}, {13, 5}, {14, 5}, {15, 5}, {17, 5}, {18, 5}, {18, 2}};
    COORDINATES a_block[12] = {{1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {14, 1},
                                {14, 2}, {14, 3}, {16, 4}};
    COORDINATES s_block[22] = {{1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 8}, {7, 8}, {8, 8}, {9, 8},
                                {10, 8}, {11, 8}, {12, 8}, {13, 8}, {14, 8}, {15, 8}, {16, 8}, {17, 8},
                                {18, 8}, {14, 3}, {15, 3}, {17, 3}, {18, 3}};
    COORDINATES d_block[14] = {{18, 1}, {18, 2}, {18, 3}, {18, 4}, {18, 5}, {18, 6}, {18, 7}, {18, 8},
                                {12, 1}, {12, 2}, {12, 3}, {12, 4}, {17, 1}, {16, 4}};
    COORDINATES obj_interact[3] = {{18, 2}, {17, 1}, {9, 8}};

    show_ui(map, *character, output_condition, 0, NULL, 0);

    // 
    while (1) { 
        if (kbhit()) { 
            c = getch(); 
        }

        if (c == 'w') {
            c = '\0';
            if(check_coordinates(w_block, 24, character->crd) == 1)
                continue;


            character->crd.y -= 1;
            show_ui(map, *character, output_condition, 0, NULL, 0);

            continue;
        } else if(c == 'a')
        {
            c = '\0';
            if(check_coordinates(a_block, 12, character->crd) == 1)
                continue;
            
            character->crd.x -= 1;
            show_ui(map, *character, output_condition, 0, NULL, 0);

            continue;
        } else if(c == 's')
        {
            c = '\0';
            if(check_coordinates(s_block, 23, character->crd) == 1)
                continue;


            character->crd.y += 1;
            show_ui(map, *character, output_condition, 0, NULL, 0); 

            continue;
        } else if(c == 'd')
        {
            c = '\0';
            if(check_coordinates(d_block, 14, character->crd) == 1)
                continue;
            

            character->crd.x += 1;
            show_ui(map, *character, output_condition, 0, NULL, 0);

            continue;
        } else if(c == 'h')
        {
            c = '\0';
            if(output_condition[1])
                output_condition[1] = false;
            else output_condition[1] = true;

            show_ui(map, *character, output_condition, 0, NULL, 0);

            continue;
        } else if(c == 'c')
        {
            c = '\0';
            if(output_condition[2])
                output_condition[2] = false;
            else output_condition[2] = true;

            show_ui(map, *character, output_condition, 0, NULL, 0);

            continue;
        } else if(c == 'e')
        {
            c = '\0';
            output_condition[3] = true;
            
            move_on_inventory(map, character, output_condition);
            show_ui(map, *character, output_condition, 0, NULL, 0);
            output_condition[3] = false;

            continue;
        } else if(c == 'f') 
        {
            c = '\0';
            int obj_index = check_obj_coordinates(ar_of_obj, 3, character->crd);

            if(obj_index != -1)
            {
                output_condition[6] = true;
                int r_code = move_on_obj_menu(map, character, output_condition, ar_of_obj[obj_index]);
                
                if(r_code == 3)
                    break;
            }

            continue;
        } else if(c == '`')
        {
            c = '\0';
            int menu_exit_code = move_in_in_game_menu(character, ar_of_obj, 2);
            
            if(menu_exit_code == 0)
                break;
            
            show_ui(map, *character, output_condition, 0, NULL, 0);
            continue;
        }
    }
}

void free_inventory(CHARACTER* character) {
    for (int i = 0; i < character->count_of_items; i++) {
        free(character->inventory[i]);
    }
    character->count_of_items = 0;
}

void game_start() {
    char c = '\0';
    int inv_cursor_location = 0;
    
    bool output_condition[9] = {true, true, true, false, true, true, false, false, false};
    /* Conditions: 
        0 - map
        1 - hints
        2 - coordinates
        3 - inventory
        4 - move up in inventory
        5 - move down in inventory
        6 - object menu
        7 - move left in obj menu
        8 - move right in obj menu
    */
    
    // Define the new character
    CHARACTER new_character = {.crd.x = 5, .crd.y = 5, {}, 0};

    OBJECT new_object_1 = {"chest\0", "Open", "You got the key!\0", "no\0", 0, {{18, 2}, {17, 1}, {-1, -1}, {-1, -1}}};
    OBJECT new_object_2 = {"exit\0", "Open", "You escaped!\0", "You need the key!\0", 0, {{9, 8}, {-1, -1}, {-1, -1}, {-1, -1}}};

    OBJECT* ar_of_objs[2] = {&new_object_1, &new_object_2};

    //Use pointers to strings for each row (allocate memory)
    char *map[10] = {
        malloc(22), malloc(22), malloc(22), malloc(22), malloc(22),
        malloc(22), malloc(22), malloc(22), malloc(22), malloc(22)
    };

    const char *initial_map[] = {
        "====================\0", 
        "|            #    ?|\0", 
        "|            #     |\0", 
        "|            #     |\0",
        "|            ### ##|\0", 
        "|                  |\0", 
        "|                  |\0", 
        "|                  |\0",
        "|                  |\0", 
        "=========E==========\0"
    };

    // Copy init_map to map
    for (int i = 0; i < 10; i++) {
        strncpy(map[i], initial_map[i], 22);
    }

    move_on_map(map, &new_character, output_condition, ar_of_objs);
    free_inventory(&new_character);
    return;
}

void load_game()
{
    char c = '\0';
    int inv_cursor_location = 0;
    
    bool output_condition[9] = {true, true, true, false, true, true, false, false, false};
    /* Conditions: 
        0 - map
        1 - hints
        2 - coordinates
        3 - inventory
        4 - move up in inventory
        5 - move down in inventory
        6 - object menu
        7 - move left in obj menu
        8 - move right in obj menu
    */

    CHARACTER loaded_character;
    int item_cnt = 0;
    int obj_cnt = 0;
    int names_cnt = 0;

    OBJECT new_object_1 = {"chest\0", "Open", "You got the key!\0", "no\0", 0, {{18, 2}, {17, 1}, {-1, -1}, {-1, -1}}};
    OBJECT new_object_2 = {"exit\0", "Open", "You escaped!\0", "You need the key!\0", 0, {{9, 8}, {-1, -1}, {-1, -1}, {-1, -1}}};

    OBJECT* ar_of_objs[2] = {&new_object_1, &new_object_2};

    FILE* f_count_of_items = fopen("sv.count_of_items", "rb");
    fread(&loaded_character.count_of_items, sizeof(int), 1, f_count_of_items);
    fclose(f_count_of_items);

    FILE* f_inventory = fopen("sv.inventory", "rb");
    while(fread(&loaded_character.inventory[item_cnt], sizeof(loaded_character.inventory[0]), 1, f_inventory) == 1)
        item_cnt++;
    fclose(f_inventory);

    FILE* f_coordinates = fopen("sv.coordinates", "rb");
    fread(&loaded_character.crd, sizeof(loaded_character.crd), 1, f_coordinates);
    fclose(f_coordinates);

    FILE* f_objects = fopen("sv.objects", "rb");
    while (obj_cnt < 2 && fread(&ar_of_objs[obj_cnt]->is_used, sizeof(bool), 1, f_objects) == 1)
        obj_cnt++;
    fclose(f_objects);

    FILE* f_items_names = fopen("sv.items_names", "rb");
    while (names_cnt < loaded_character.count_of_items && fread(&loaded_character.inventory[names_cnt]->name, sizeof(char) * 15, 1, f_items_names) == 1)
        names_cnt++;
    fclose(f_items_names);

    //Use pointers to strings for each row (allocate memory)
    char *map[10] = {
        malloc(22), malloc(22), malloc(22), malloc(22), malloc(22),
        malloc(22), malloc(22), malloc(22), malloc(22), malloc(22)
    };

    const char *initial_map[] = {
        "====================\0", 
        "|            #    ?|\0", 
        "|            #     |\0", 
        "|            #     |\0",
        "|            ### ##|\0", 
        "|                  |\0", 
        "|                  |\0", 
        "|                  |\0",
        "|                  |\0", 
        "=========E==========\0"
    };

    // Copy init_map to map
    for (int i = 0; i < 10; i++) {
        strncpy(map[i], initial_map[i], 22);
    }

    move_on_map(map, &loaded_character, output_condition, ar_of_objs);
    free_inventory(&loaded_character);
    return;
}

void game_save(CHARACTER* character, OBJECT* ar_of_objects[], int count_of_objs) {
    bool obj_stats[] = {ar_of_objects[0]->is_used, ar_of_objects[1]->is_used};
    char** items_names = malloc(sizeof(char*) * character->count_of_items);

    for(int i = 0; i < character->count_of_items; i++)
        items_names[i] = malloc(sizeof(char) * 15);
    
    for(int i = 0; i < character->count_of_items; i++)
        strncpy(items_names[i], character->inventory[i]->name, 15);

    FILE* f_inventory = fopen("sv.inventory", "wb");

    if(f_inventory == NULL) {
        perror("sv.inventory");
        return;
    }

    fwrite(&character->inventory, sizeof(character->inventory), character->count_of_items, f_inventory);
    fclose(f_inventory);

    FILE* f_coordinates = fopen("sv.coordinates", "wb");

    if(f_coordinates == NULL) {
        perror("sv.coordinates");
        return;
    }

    fwrite(&character->crd, sizeof(character->crd), 1, f_coordinates);
    fclose(f_coordinates);

    FILE* f_count_of_items = fopen("sv.count_of_items", "wb");

    if(f_count_of_items == NULL) {
        perror("sv.count_of_items");
        return;
    }

    fwrite(&character->count_of_items, sizeof(character->count_of_items), 1, f_count_of_items);
    fclose(f_count_of_items);
 
    FILE* f_objects = fopen("sv.objects", "wb");

    if(f_objects == NULL) {
        perror("sv.objects");
        return;
    }

    fwrite(&obj_stats, sizeof(bool), count_of_objs, f_objects);
    fclose(f_objects);

    FILE* f_items_names = fopen("sv.items_names", "wb");
    fwrite(&items_names, sizeof(char) * 15, character->count_of_items, f_items_names);
    fclose(f_items_names);

    int menu_type = 1;
    FILE* f = fopen("menu_type.game", "wb");
    fwrite(&menu_type, sizeof(menu_type), 1, f);
    fclose(f);

    for(int i = 0; i < character->count_of_items; i++)
        free(items_names[i]);
    free(items_names);

    printf("\nGAME SAVED!\n");

    return;
}