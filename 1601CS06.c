#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#define bsize 5

//mainsleep = time main() runs
//nprod, ncons = number of producers and consumers
//modint = rand() % item value
//cmodsleep, pmodsleep = rand() % sleep times consumer, producer
//lock = buffer mutex
static int mainsleep, nprod, ncons, modint = 10, cmodsleep = 3, pmodsleep = 5;
static int empty = bsize, full = 0, pos = 0;
static int buffer[bsize];
pthread_mutex_t lock;

//function to insert item
int insert_item(int a)
{
	if(empty == 0 && full == bsize)
		return -1;
	else
	{
		buffer[pos] = a;
		pos++;
		empty--;
		full++;
		return 0;
	}
}

//function to remove item
int remove_item()
{
	if(full == 0 && empty == bsize)
		return -1;
	else
	{
		empty++;
		full--;
		return buffer[--pos];
	}
}

//main producer function
void* producer(void* num)
{
	int item, status;
	while(1)
	{
		item = rand()%modint;
		pthread_mutex_lock(&lock);
		status = insert_item(item);
		if(status != -1)printf("Producer %d has inserted integer %d\n", (int)num, item);
		else printf("Producer %d failed to insert item as buffer was full\n", (int)num);
		pthread_mutex_unlock(&lock);
		sleep(rand()%pmodsleep);
	}
}

//main consumer function
void* consumer(void* num)
{
	int item;
	while(1)
	{
		pthread_mutex_lock(&lock);
		item = remove_item();
		if(item != -1)printf("Consumer %d has retrieved integer %d\n", (int)num, item);
		else printf("Consumer %d failed to retrieve item as buffer was empty\n", (int)num);
		pthread_mutex_unlock(&lock);
		sleep(rand()%cmodsleep);
	}
}

int main(int argc, char* argv[])
{
	if(argc < 4)
	{
		printf("Enter all parameters\n");
		return 0;
	}

	mainsleep = atoi(argv[1]);
	nprod = atoi(argv[2]);
	ncons = atoi(argv[3]);

	//initialize mutex
	if (pthread_mutex_init(&lock, NULL) != 0) 
    { 
        printf("Mutex init has failed\n");
        return 0; 
    }

    //initialize threads
	pthread_t prodthread[nprod], consthread[ncons];

	for (int i = 0; i < nprod; i++)
	{
		if(pthread_create(&prodthread[i], NULL, producer, (void *)i) != 0)
			printf("Producer thread initialization failed\n");
	}

	for (int i = 0; i < ncons; i++)
	{
		if(pthread_create(&consthread[i], NULL, consumer, (void *)i) != 0)
			printf("Consumer thread initialization failed\n");
	}

	//main program sleeping
	sleep(mainsleep);
	//exit
	printf("Program ending...\n");
	return 0;
}
