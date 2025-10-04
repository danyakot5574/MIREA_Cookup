#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void* threadFunction(void* data)
{
    int number = ((int*)data)[0];
    int time = ((int*)data)[1];
    printf("Thread %d started!\n", number);

    sleep(time);

    printf("Thread %d finished!\n", number);

    return NULL;
}

int printHelp()
{
    printf("\nHelp menu\n");
    printf("\t-h (--help)   - show this menu\n");
    printf("\t-x <int>[args] (--exec <int>[args]) - run a thread pattern [args]\n");
    printf("\t-r (--replay) - replay the last executed pattern from last.bin\n\n");
    return 0;
}

int askUserRepeat(int count, int data[][2])
{
    while (1) {
        printf("\nEnter 'r' to replay this pattern, 'x' to exit: ");
        char choice[8];
        if (fgets(choice, sizeof(choice), stdin) == NULL)
            continue;

        if (choice[0] == 'r' || choice[0] == 'R') {
            pthread_t threads[count];
            for (int i = 0; i < count; i++) {
                int result = pthread_create(&threads[i], NULL, threadFunction, &data[i]);
                if (result != 0) {
                    perror("Thread creation error");
                    return 0;
                }
            }
            for (int i = 0; i < count; i++)
                pthread_join(threads[i], NULL);
        }
        else if (choice[0] == 'x' || choice[0] == 'X') {
            printf("Exiting program.\n");
            exit(0);
        }
        else {
            printf("Invalid input! Please try again.\n");
        }
    }
}


int runPattern(int *argc, char **argv)
{
    if (*argc < 3)
        return 1;

    for (int i = 0; i < *argc - 2; ++i) {
        char *arg = argv[i + 2];
        for (int j = 0; j < (int)strlen(arg); j++) {
            if (!isdigit(arg[j])) {
                return 1; 
            }
        }
        int val = atoi(arg);
        if (val < 0) {
            return 1;
        }
    }

    int count = *argc - 2;
    pthread_t threads[count];
    int data[count][2];

    for (int i = 0; i < count; i++) {
        data[i][0] = i + 1;
        data[i][1] = atoi(argv[i + 2]);
    }

    for (int i = 0; i < count; i++) {
        int result = pthread_create(&threads[i], NULL, threadFunction, &data[i]);
        if (result != 0) {
            perror("Thread creation error");
            return 0;
        }
    }

    for (int i = 0; i < count; i++)
        pthread_join(threads[i], NULL);

    FILE *f = fopen("last.bin", "wb");
    if (f != NULL) {
        fwrite(&count, sizeof(int), 1, f);
        fwrite(data, sizeof(int), count * 2, f);
        fclose(f);
    } else {
        perror("Could not open last.bin for writing");
    }

    askUserRepeat(count, data);

    return 0;
}

int replayPattern()
{
    FILE *f = fopen("last.bin", "rb");
    if (f == NULL) {
        printf("No last.bin found!\n");
        return 1;
    }

    int count;
    if (fread(&count, sizeof(int), 1, f) != 1) {
        fclose(f);
        printf("Corrupted last.bin!\n");
        return 1;
    }

    int data[count][2];
    if (fread(data, sizeof(int), count * 2, f) != (size_t)(count * 2)) {
        fclose(f);
        printf("Corrupted last.bin!\n");
        return 1;
    }
    fclose(f);

    pthread_t threads[count];
    for (int i = 0; i < count; i++) {
        int result = pthread_create(&threads[i], NULL, threadFunction, &data[i]);
        if (result != 0) {
            perror("Thread creation error");
            return 0;
        }
    }

    for (int i = 0; i < count; i++)
        pthread_join(threads[i], NULL);

    askUserRepeat(count, data);

    return 0;
}

int initArg(int *argc, char **argv)
{
    int result = 2;

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        result = printHelp();
    }
    else if (strcmp(argv[1], "-x") == 0 || strcmp(argv[1], "--exec") == 0) {
        result = runPattern(argc, argv);
    }
    else if (strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "--replay") == 0) {
        result = replayPattern();
    }

    if (result == 1 || result == 2)
        printf("Bad usage!\nTry './test -h' for more information.\n");

    return 0;
}

int main(int argc, char** argv)
{
    if (argc > 1)
        initArg(&argc, argv);
    else
        printf("Bad usage!\nTry './test -h' for more information.\n");    

    return 0;
}

