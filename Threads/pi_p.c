#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<time.h>
#define THREAD_NUM 4
#define INTERVALOS 10000000000

pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;
double acum = 0;
double base;
time_t start, finish;
void* pi(void* arg){
	double*param = (double*) arg;
	double fdx;
	double x;
	long i;
	double acum2= 0;
	x = (INTERVALOS/THREAD_NUM)*base*(*param);
	//printf("%f\n", x);
	for (long i = 0; i<(INTERVALOS/THREAD_NUM); i++){
		fdx = 4/(1+x*x);
		acum2 += fdx*base;
		x+=base;
	}
	pthread_mutex_lock(&mutexsum);
	acum+=acum2;
	pthread_mutex_unlock(&mutexsum);
}

int main(){
	double elapsed;
	//struct timespec start, finish;
	double y[THREAD_NUM];
	printf("%d\n", THREAD_NUM);
	//pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;
	pthread_t thread[THREAD_NUM];
	base = 1.0/INTERVALOS;
	start = clock();
	for(long i = 0; i<THREAD_NUM; i++){
		y[i] = i;
		pthread_create(&thread[i], NULL, pi, (void*)&y[i]);
	}
	
	for( int i = 0; i<THREAD_NUM; i++){
		pthread_join(thread[i], NULL);
	}
	finish = clock();
	elapsed = finish-start;
	
	printf("Resultado: %20.18lf (%20.18lf)\n", acum, (elapsed));
return 0;
}
