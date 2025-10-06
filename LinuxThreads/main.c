#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define THREAD_COUNT 5
#define SAVE_FILE "last.bin"

void* threadFunction(void* data)
{
    int number = ((int*)data)[0];
    int time = ((int*)data)[1];
    printf("Thread %d started! (duration: %d sec)\n", number, time);
    sleep(time);
    printf("Thread %d finished!\n", number);
    return NULL;
}

void printHelp()
{
    printf("\nHelp menu:\n");
    printf("  -h, --help     Show this help message\n");
    printf("  -x, --exec     Run a new 5-thread pattern (you will enter timing for each thread)\n");
    printf("  -r, --replay   Replay the last successfully executed thread pattern from %s\n\n", SAVE_FILE);
}

int askUserRepeat(int times[THREAD_COUNT]);

int runPattern()
{
    int times[THREAD_COUNT];

    // Ask user for 5 timings
    printf("Enter timings (in seconds) for %d threads:\n", THREAD_COUNT);
    for (int i = 0; i < THREAD_COUNT; i++) {
        while (1) {
            printf("  Thread %d: ", i + 1);
            char input[32];
            if (!fgets(input, sizeof(input), stdin)) {
                printf("Input error! Try again.\n");
                continue;
            }

            // remove newline
            input[strcspn(input, "\n")] = 0;

            // check that input is a valid positive number
            int valid = 1;
            for (int j = 0; j < (int)strlen(input); j++) {
                if (!isdigit(input[j])) {
                    valid = 0;
                    break;
                }
            }

            if (!valid || strlen(input) == 0) {
                printf("Invalid input! Please enter a positive number.\n");
                continue;
            }

            int val = atoi(input);
            if (val <= 0) {
                printf("Invalid input! Value must be greater than zero.\n");
                continue;
            }

            times[i] = val;
            break;
        }
    }

    // Run threads
    pthread_t threads[THREAD_COUNT];
    int data[THREAD_COUNT][2];

    for (int i = 0; i < THREAD_COUNT; i++) {
        data[i][0] = i + 1;
        data[i][1] = times[i];
    }

    printf("\nStarting threads...\n");
    for (int i = 0; i < THREAD_COUNT; i++) {
        int result = pthread_create(&threads[i], NULL, threadFunction, &data[i]);
        if (result != 0) {
            perror("Thread creation error");
            return 1;
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++)
        pthread_join(threads[i], NULL);

    printf("\nAll threads have finished!\n");

    // Save pattern
    FILE *f = fopen(SAVE_FILE, "wb");
    if (f == NULL) {
        perror("Failed to open file for writing");
        return 1;
    }
    fwrite(times, sizeof(int), THREAD_COUNT, f);
    fclose(f);

    printf("Pattern successfully saved to %s.\n", SAVE_FILE);

    askUserRepeat(times);
    return 0;
}

int replayPattern()
{
    FILE *f = fopen(SAVE_FILE, "rb");
    if (f == NULL) {
        printf("No saved pattern found! Run the program with -x first.\n");
        return 1;
    }

    int times[THREAD_COUNT];
    if (fread(times, sizeof(int), THREAD_COUNT, f) != THREAD_COUNT) {
        fclose(f);
        printf("Corrupted pattern file!\n");
        return 1;
    }
    fclose(f);

    pthread_t threads[THREAD_COUNT];
    int data[THREAD_COUNT][2];

    for (int i = 0; i < THREAD_COUNT; i++) {
        data[i][0] = i + 1;
        data[i][1] = times[i];
    }

    printf("\nReplaying last pattern...\n");
    for (int i = 0; i < THREAD_COUNT; i++) {
        int result = pthread_create(&threads[i], NULL, threadFunction, &data[i]);
        if (result != 0) {
            perror("Thread creation error");
            return 1;
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++)
        pthread_join(threads[i], NULL);

    printf("\nReplay finished!\n");

    askUserRepeat(times);
    return 0;
}

int askUserRepeat(int times[THREAD_COUNT])
{
    while (1) {
        printf("\nEnter 'r' to replay this pattern again, or 'x' to exit: ");
        char choice[8];
        if (!fgets(choice, sizeof(choice), stdin))
            continue;

        if (choice[0] == 'r' || choice[0] == 'R') {
            // replay the same pattern
            pthread_t threads[THREAD_COUNT];
            int data[THREAD_COUNT][2];
            for (int i = 0; i < THREAD_COUNT; i++) {
                data[i][0] = i + 1;
                data[i][1] = times[i];
            }

            printf("\nReplaying current pattern...\n");
            for (int i = 0; i < THREAD_COUNT; i++) {
                int result = pthread_create(&threads[i], NULL, threadFunction, &data[i]);
                if (result != 0) {
                    perror("Thread creation error");
                    return 1;
                }
            }

            for (int i = 0; i < THREAD_COUNT; i++)
                pthread_join(threads[i], NULL);

            printf("\nPattern finished!\n");
        }
        else if (choice[0] == 'x' || choice[0] == 'X') {
            printf("Exiting program.\n");
            exit(0);
        }
        else {
            printf("Invalid input! Please enter 'r' or 'x'.\n");
        }
    }
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Bad usage!\nTry './test -h' for more information.\n");
        return 0;
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        printHelp();
    }
    else if (strcmp(argv[1], "-x") == 0 || strcmp(argv[1], "--exec") == 0) {
        runPattern();
    }
    else if (strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "--replay") == 0) {
        replayPattern();
    }
    else {
        printf("Unknown option '%s'.\nTry './test -h' for help.\n", argv[1]);
    }

    return 0;
}
