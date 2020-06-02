#include <stdio.h>
#include <time.h>

#define STEPS 2000000000
#define BLOCKS 100
#define THREADS 100

int threadidx;
double pi = 0;
clock_t start, end;

__device__ float pi_global;
// Kernel
__global__ void pi_calculation( int nsteps, double base, int nthreads, int nblocks)
{
    float accum =0;
int i;
float x;
int idx = blockIdx.x * blockDim.x + threadIdx.x; // Calculate index for each thread
for (i = idx; i < nsteps; i += nthreads * nblocks)
{
x = (i + 0.5) * base;
accum += 4.0 / (1.0 + x * x);

//sum[idx] += 4.0 / (1.0 + x * x); //Save result to device memory
}
atomicAdd(&pi_global, accum);
}

int main(void)
{
dim3 dimGrid(BLOCKS, 1, 1); // Grid dimensions
dim3 dimBlock(THREADS, 1, 1); // Block dimensions

double base = 1.0 / STEPS; // base size
float pi_result = 0;



start = clock();
// Launch Kernel
pi_calculation << <dimGrid, dimBlock >> > (STEPS, base, THREADS, BLOCKS);

// Sync
cudaDeviceSynchronize();




cudaMemcpyFromSymbol(&pi_result, pi_global, sizeof(float));
pi_result *= base;
end = clock();
// Output Results
printf("PI = %.10f\n (%ld)", pi_result, end-start);


return 0;
}