#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#define n 5

pthread_t thread[n];
sem_t forks[n];
int req[n][n], alloc[n][n], fstat[n], finished[n], eat;

void* phil(void* num)
{
	int pid = (int)num;
	int l = pid%n;
	int r = (pid+1)%n;

	while(1)
	{
		req[pid][r] = req[pid][l];
		printf("Philosopher %d is thinking\n", pid);
		fflush(stdout);
		sleep(1);

		printf("Philosopher %d is hungry\n", pid);
		printf("Philosopher %d requests left-fork-%d\n", pid, l);
		fflush(stdout);
		req[pid][l] = 1;
		sem_wait(&forks[l]);
		fstat[l] = pid;
		printf("Philosopher %d grabbed left-fork-%d\n", pid, l);
		fflush(stdout);
		alloc[pid][l] = 1;
		req[pid][l] = 0;

		sleep(1);

		if(!alloc[pid][l])continue;

		printf("Philosopher %d requests right-fork-%d\n", pid, r);
		fflush(stdout);
		req[pid][r] = 1;
		sem_wait(&forks[r]);
		fstat[r] = pid;
		printf("Philosopher %d grabbed right-fork-%d\n", pid, r);
		fflush(stdout);
		alloc[pid][r] = 1;
		req[pid][r] = 0;
		printf("Philosopher %d is now eating\n", pid);
		fflush(stdout);
		eat = eat + 1;
		sleep(1);

		printf("Request matrix:\n");
		for(int i = 0;i < n; i++)
		{
			for(int j = 0;j < n;j++)
				printf("%d ", req[i][j]);
			printf("\n");
		}

		printf("Allocation matrix:\n");
		for(int i = 0;i < n; i++)
		{
			for(int j = 0;j < n;j++)
				printf("%d ", alloc[i][j]);
			printf("\n");
		}

		printf("Philosopher %d has eaten. Releasing left-fork-%d and right-fork-%d\n", pid, l, r);
		fflush(stdout);
		fstat[l] = fstat[r] = -1;
		alloc[pid][l] = alloc[pid][r] = 0;
		eat = eat - 1;
		sem_post(&forks[l]);
		sem_post(&forks[r]);
		break;
	}
	finished[pid] = 1;
	pthread_exit((void *)0);
}

void dl()
{
	if(eat > 0)return;
	int i;
	for(i = 0;i < n; i++)
		if(fstat[i] == -1)return;
	sleep(1);
	if(eat > 0)return;

	printf("Parent detects deadlock, going to initiate recovery.\n");
	fflush(stdout);

	printf("Request matrix:\n");
	for(int i = 0;i < n; i++)
	{
		for(int j = 0;j < n;j++)
			printf("%d ", req[i][j]);
		printf("\n");
	}

	printf("Allocation matrix:\n");
	for(int i = 0;i < n; i++)
	{
		for(int j = 0;j < n;j++)
			printf("%d ", alloc[i][j]);
		printf("\n");
	}

	int kill = rand() % 5;
	printf("Parent pre-empts Philosopher %d\n", kill);
	fflush(stdout);

	alloc[kill][kill%n] = 0;
	sem_post(&forks[kill%n]);
	return;
}

int main()
{
	for (int i = 0; i < n; ++i)sem_init(&forks[i], 0, 1);
	memset(fstat, -1, sizeof(fstat));
	memset(alloc, 0, sizeof(alloc));
	memset(req, 0, sizeof(req));
	memset(finished, 0, sizeof(finished));

	for (int i = 0; i < n; i++)pthread_create(&thread[i], NULL, phil, (void *)i);

	while(1)
	{
		dl();
		int completed = 0;
		for (int i = 0; i < n; ++i)completed += finished[i];
		if(completed == 5)break;
	}

	for(int i = 0; i < n; i++)pthread_join(thread[i], NULL);

	for (int i = 0; i < n; ++i)sem_destroy(&forks[i]);
	return 0;
}