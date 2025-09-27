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
	
	printf("Thread %d ended!\n", number);
	
	return NULL;
}


int printHelp()
{
	printf("\nHelp menu\n\t-h (--help) - show this menu\n\t-r <int>[args] (--run <int>[args]) - run pattern [args]\n\n");
	return 0;
}

int runPattern(int * argc, char ** argv)
{
	if(*argc < 3)
		return 1;

	for(int i = 0; i < *argc - 2; ++i)
		if(isdigit(*(argv[i + 2])) == 0)
			return 1;

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
			return 0;
		}
	}

	for(int i = 0; i < *argc - 2; i++)
		pthread_join(threads[i], NULL);

	return 0;
}

int initArg(int * argc, char ** argv)
{
	int result = 2;

	if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
	{
		result = printHelp();
	}
	else if(strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "--run") == 0)
	{
		result = runPattern(argc, argv);
	}

	if(result == 1 || result == 2)
		printf("Bad usage!\nTru './test -h' for more information.\n");

	return 0;
}

int main(int argc, char** argv)
{
	if(argc > 1)
		initArg(&argc, argv);
	else
		printf("Bad usage!\nTry './test -h' for more information.\n");	
	
	return 0;
}
