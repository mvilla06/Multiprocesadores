#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void *fThread_routine (void *arg){

	int* param = (int*) arg;
	printf("Hello world from thread %d\n", *param);
}

int main(){
	pthread_t thread_ID[4];
	int y[4];
	for (int i = 0; i<4; i++){
		y[i] = i;
		pthread_create(&thread_ID[i], NULL, fThread_routine, (void*)&y[i]);
	}
	
	for (int i = 0; i<4; i++){
		pthread_join(thread_ID[i], NULL);
	}
return 0;
}

