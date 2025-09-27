#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void* threadFunction(void* data)
{
	int number = ((int*)data)[0];
	int time = ((int*)data)[1];
	printf("Thread %d started!\n", number);
	
	sleep(time);
	
	printf("Thread %d ended!\n", number);
	
	return NULL;
}


void printHelp()
{
	printf("\nHelp menu\n\t-h (--help) - show this menu\n\t-r [args] (--run [args]) - run pattern [args]\n\n");
}

void runPattern(int * argc, char ** argv)
{
	pthread_t threads[*argc - 1];
	int data[*argc - 1][2];

	for(int i = 0; i < *argc - 2; i++)
	{
		data[i][0] = i + 1;
		data[i][1] = atoi(argv[i + 2]);
	}

	for(int i = 0; i < *argc - 2; i++)
	{
		int result = pthread_create(&threads[i], NULL, threadFunction, &data[i]);
		
		if(result != 0)
		{
			perror("Ошибка создания потока\n");
			return;
		}
	}

	for(int i = 0; i < *argc - 2; i++)
		pthread_join(threads[i], NULL);
}

int initArg(int * argc, char ** argv)
{
	if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
	{
		printHelp();
		return 0;
	}

	if(strcmp(argv[1], "-p") == 0 || strcmp(argv[1], "--run") == 0)
	{
		runPattern(argc, argv);
		return 0;
	}
}

int main(int argc, char** argv)
{
	printf("Start\n");

	if(argc > 1)
		initArg(&argc, argv);
	else
		printf("Bad usage!\nTry './test -h' for more information.\n");	

	printf("End\n");

	return 0;
}
