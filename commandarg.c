#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_LENGTH 1024
int createdcount = 0;
int created = 0;
int itemsused = 0;
sem_t sem;
int prodstobemade;
int constobemade;
int bufferstobemade;
int itemstobemade;
int **buffers;
int *buffsize;

void* produce (void* unsued){
	//have the thread die every 1000 items created, will run reader, then
	//restart the threads agian. Once total items is reached, close thread
	while (createdcount<1000 && created < itemstobemade){
		if (sem_wait(&sem) == 0) {
		//prod/cons algorithm goes here
		created++;
		}
	}
	return NULL;
}
void* consume (void* unsued){
	//similar logic to producer.
	while (createdcount<1000 && itemsused < itemstobemade){
		if (sem_wait(&sem) == 0){
		//blah blah algorithm here
		itemsused++;
		}
	}
	return NULL;
}

int main (int argc, char** argv){
	argv++;
	//skipped program name, grabbing commandline args and converting to ints
	sscanf(*argv,"%d", &prodstobemade); argv++;
	sscanf(*argv,"%d", &constobemade); argv++;
	sscanf(*argv,"%d", &bufferstobemade); argv++;
	sscanf(*argv,"%d", &itemstobemade); argv++;
	//init the semaphore for the number of buffers to be used
	sem_init(&sem, 0, bufferstobemade);
	//allocate the buffers that are needed and track their size
	buffsize = (int*) malloc(bufferstobemade);
	buffers = calloc (bufferstobemade, sizeof(int *));
	int i;
	for (i = 0 ; i < bufferstobemade;i++){
		buffers[i] = calloc(BUFFER_LENGTH, sizeof(int));
		buffsize[i] = 0;
	}
	//set up thread groupings to easily manage them once they have started
	pthread_t *cons = (pthread_t*)malloc(constobemade);
	pthread_t *prods = (pthread_t*)malloc(prodstobemade);
	pthread_t thread_id;
	//start up each thread
  	//pthread_create (&thread_id, NULL, &print_xs, NULL);
  	//temporarily have a finish, will work out at end.
  	pthread_join(thread_id, NULL);
  	pthread_join(thread_id2, NULL);
	return 0;
}