#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_LENGTH 1024
int created = 0;
int itemsused = 0;
sem_t empty;
sem_t full;
int prodstobemade;
int constobemade;
int bufferstobemade;
int itemstobemade;
int **buffers;
int *buffsize;

void* produce (void* unsued){
	int a = *((int *)unsued);
	int i;
	while(created < itemstobemade){
		sem_wait(&empty);
		for (i = 0; i<bufferstobemade; i++){
			while (*(buffsize + i) < 1024){
				created++;
				(*(buffsize+i))++;
				//*((*(buffers + i))+(*buffsize + i) to access the element to the array
			}
		}
		sem_post(&full);
	}
	printf("Producer thread %i is done\n",(a+1));
}
void* consume (void* unsued){
	int a = *((int *)unsued);
	int i;
	while(itemsused < itemstobemade){
		sem_wait(&full);
		for (i = 0; i<bufferstobemade; i++){
			while (*(buffsize + i) > 0){
				itemsused++;
				(*(buffsize+i))--;
				//*((*(buffers + i))+(*buffsize + i) to access the element to the array
			}
		}
		sem_post(&empty);
	}
	printf("Consumer thread %i is done\n",(a+1));
}
void* bufferPrinter (void* unsued){
	printf("hello");
}

int main (int argc, char** argv){
	argv++;
	//skipped program name, grabbing commandline args and converting to ints
	sscanf(*argv,"%d", &prodstobemade); argv++;
	sscanf(*argv,"%d", &constobemade); argv++;
	sscanf(*argv,"%d", &bufferstobemade); argv++;
	sscanf(*argv,"%d", &itemstobemade); argv++;
	//init the semaphore for the number of buffers to be used
	sem_init(&empty, 1, 1);
	sem_init(&full, 1, 0);
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
	//start up each thread
	for ( i = 0; i < constobemade ; i++){
		int *arg = malloc(sizeof(*arg));
		if (arg == NULL){
			fprintf(stderr, "Couldn't allocate int.\n");
			exit(EXIT_FAILURE);
		}
		*arg = i;
		pthread_create((cons + i), 0, consume, arg);
	}
	for ( i = 0; i < prodstobemade ; i++){
		int *arg = malloc(sizeof(*arg));
		if (arg == NULL){
			fprintf(stderr, "Couldn't allocate int.\n");
			exit(EXIT_FAILURE);
		}
		*arg = i;
		pthread_create((prods + i), 0, produce, arg);
	}
	//wait for program to finish
  	for ( i = 0 ; i < constobemade ; i++ )
  		pthread_join(*(cons + i), NULL);
  	for ( i = 0 ; i < prodstobemade ; i++ )
  		pthread_join(*(prods + i), NULL);
	return 0;
}