#include <stdio.h>
#include <string.h>

void login_user();
void reg_user();
void change_pass();
void del_user();

enum {
    max_pass_log_length = 20
};

typedef enum {
    log = 1, reg, change, del, ext
} CHOOSER;

int main(void)
{
    char* ptr;

    while (1)
    {
        CHOOSER chooser;
        printf("Hello!\n1) Sign In\n2) Sign Up\n3) Change password\n4) Delete user \n5) Exit\n");
        scanf("%d", &chooser);

        switch (chooser)
        {
        case log:
            login_user();
            break;
        
        case reg:
            reg_user();
            break;

        case change:
            change_pass();
            break;

        case del:
            del_user();
            break;
        
        case ext:
            printf("Bye!");
            return 0;
        
        default:
            printf("There is no such command!\n");
            break;
        }
    }
    
    
    return 0;
}

void login_user()
{
    char* ptr;
    char login[max_pass_log_length], true_password[max_pass_log_length], input_password[max_pass_log_length];
    printf("Input login: ");
    scanf("%s", login);

    FILE* fp = fopen(login, "r");

    if(fp == NULL)
    {
        printf("There is no such user!\n\n");
        return;
    }

    fgets(true_password, max_pass_log_length, fp);
    if((ptr = strchr(true_password, '\n')) != NULL)
        *ptr = '\0';

    for(int i = 0; i < 3; i++)
    {
        printf("Input password (Attempts left - %d): ", 3 - i);
        scanf("%s", &input_password);

        if(strstr(input_password, true_password) != NULL && strstr(true_password, input_password) != NULL)
        {
            printf("Success!\n\n");
            fclose(fp);
            return;
        }
    }

    printf("Incorrect!\n\n");
    fclose(fp);
}

void reg_user()
{
    char login[max_pass_log_length], password[max_pass_log_length], repeat[max_pass_log_length];
    printf("Creating new user!\nInput new login: ");
    scanf("%s", &login);

    FILE* fp = fopen(login, "r");

    if(fp != NULL)
    {
        printf("This user is already exist!\n\n");
        fclose(fp);
        return;
    }
    fclose(fp);

    FILE* wr = fopen(login, "w+");

    for(int i = 0; i < 3; i++)
    {
        printf("Input password: ");
        scanf("%s", &password);

        printf("Repeat: ");
        scanf("%s", &repeat);

        if (strstr(repeat, password) == NULL || strstr(password, repeat) == NULL)
        {
            printf("Passwords don't match! (Attempts left - %d)\n", 3 - (i + 1));
        } else {
            printf("New user is successfull created!\n\n");
            fputs(password, wr);
            fclose(wr);
            return;
        }
    }

    printf("Failure!\n\n");
    fclose(wr);
    remove(login);
    return;
}

void change_pass()
{
    char login[max_pass_log_length], password[max_pass_log_length], input_password[max_pass_log_length], repeat[max_pass_log_length], new_password[max_pass_log_length];
    char* ptr;

    printf("Input login: ");
    scanf("%s", &login);

    FILE* fp = fopen(login, "r");
    if(fp == NULL)
    {
        printf("There is no such user!\n\n");
        fclose(fp);
        return;
    }
    fgets(password, max_pass_log_length, fp);

    if((ptr = strchr(password, '\n')) != NULL)
        *ptr = '\0';

    for(int i = 0; i < 3; i++)
    {
        printf("Input password (Attempts left - %d): ", 3 - i);
        scanf("%s", &input_password);

        if(strstr(input_password, password) != NULL && strstr(password, input_password) != NULL)
        {
            printf("Success!\n\n");
            fclose(fp);

            for(int i = 0; i < 3; i++)
            {
                printf("Input NEW password: ");
                scanf("%s", &password);
                printf("Repeat NEW passord: ");
                scanf("%s", &repeat);

                if (strstr(repeat, password) == NULL || strstr(password, repeat) == NULL)
                {
                printf("Passwords don't match! (Attempts left - %d)\n", 3 - (i + 1));
                } else {
                    printf("New password is successfull created!\n\n");
                    fp = fopen(login, "w");
                    fputs(password, fp);
                    fclose(fp);
                return;
                }
            }

            printf("Failure!\n\n");
            fclose(fp);
            return;
        }
    }

    printf("Incorrect!\n\n");
    fclose(fp);
    return;
}

void del_user()
{
    char login[max_pass_log_length], true_password[max_pass_log_length], input_password[max_pass_log_length];
    int a;
    char* ptr;

    printf("Input login: ");
    scanf("%s", login);
    FILE* fp = fopen(login, "r");

    if(fp == NULL)
    {
        printf("There is no such user!\n\n");
        return;
    }

    fgets(true_password, max_pass_log_length, fp);
    fclose(fp);
    if((ptr = strchr(true_password, '\n')) != NULL)
            *ptr = '\0';

    printf("You really want to delete user? (1 - yes)\n");
    scanf("%d", &a);

    if(a == 1)
    {
        for(int i = 0; i < 3; i++)
        {
            printf("Input password (Attempts left - %d): ", 3 - i);
            scanf("%s", &input_password);

            if(strstr(input_password, true_password) != NULL && strstr(true_password, input_password) != NULL)
            {
                printf("Success! This user no longer exist...\n\n");
                remove(login);
                return;
            }
        }

        printf("Incorrect!\n\n");
    }
}