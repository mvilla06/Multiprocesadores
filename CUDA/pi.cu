#include <stdio.h>
#include <time.h>
#define NUM_INTERVALS 1000000
#define THREADS_PER_BLOCK 1000
#define BLOCKS 10

double baseIntervalo = 1.0/NUM_INTERVALS;

__global__ void piFunc(double * acum){
    int t = threadIdx.x;
    int threads = blockDim.x;
    
    acum[t] = 0;
    
    int intervals_per_thread = NUM_INTERVALS/(BLOCKS*THREADS_PER_BLOCK);
    double baseIntervalo = 1.0/NUM_INTERVALS;
    
    
    for(int i = (threads*blockIdx.x + t)*intervals_per_thread; i<(threads*blockIdx.x + t)*intervals_per_thread + intervals_per_thread; i++){
        
        double x = (i+0.5)*baseIntervalo;
        double fdx = 4 / (1 + x * x);
        acum[threads*blockIdx.x + t] += fdx;
    }

}

int main(){
    clock_t start, end;
    double h_pi[BLOCKS*THREADS_PER_BLOCK];
    double * d_pi;
    double pi = 0;

    cudaMalloc(&d_pi, BLOCKS*THREADS_PER_BLOCK*sizeof(double));


    start = clock();
    piFunc<<<BLOCKS, THREADS_PER_BLOCK>>>(d_pi);
    cudaMemcpy(h_pi, d_pi, BLOCKS*THREADS_PER_BLOCK*sizeof(double), cudaMemcpyDeviceToHost);
    
    for(int i=0; i<BLOCKS*THREADS_PER_BLOCK; i++)
        //printf("%f", h_pi[i]);
        pi+=h_pi[i];

    pi *= baseIntervalo;
    end = clock();
    printf("Result = %20.18lf (%ld)\n", pi, end - start);

    cudaFree(d_pi);


    return 0;
}