#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <semaphore.h>

int size = 50;

sem_t el, ol;

void * et(void *a)
{
   int i = 0;
   while (i < size)
   {
      sem_wait(&el);
      printf("%d\n", i);
      i+=2;
      sem_post(&ol);
    }
    pthread_exit(0);
}

void ot()
{
    int i = 1;
    while (i < size)
    {
        sem_wait(&ol);
        printf("%d\n", i);
        i+=2;
        sem_post(&el);
    }
    pthread_exit(0);
}

int main()
{
	sem_init(&el, 0, 1);
  sem_init(&ol, 0, 0);

  pthread_t thread;

	pthread_create(&thread, NULL, et, NULL);
	ot();
	pthread_join(thread, NULL);

	return 0;
}