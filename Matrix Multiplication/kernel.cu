#include <time.h>
#include <stdio.h>

__global__ void kernel(const double * A, const double * B, double * C, int rows, int cols, int k){
  //  int blockId = blockIdx.x + blockIdx.y * gridDim.x;
    //int threadId = blockId * (blockDim.x * blockDim.y) + (threadIdx.y * blockDim.x) + threadIdx.x;
    int i = blockIdx.y * blockDim.y + threadIdx.y;
    int j = blockIdx.x * blockDim.x + threadIdx.x;
    
    if(i>=rows || j>=cols)
        return;
    
    
    for(int x=0; x<k; x++)
       
        C[i * cols + j] += A[i * k + x] * B[j * k + x]; 

}

extern "C" long int kernel_wrapper(const double * A, const double * B, double * C, int rowA, int colA, int rowB, int colB){
    time_t start, end;
    double * d_A;
    double * d_B;
    double * d_C;
    cudaError_t error;
    
    error = cudaMalloc((void**)&d_A, rowA * colA * sizeof(double));
    error = cudaMalloc((void**)&d_B, rowB * colB * sizeof(double));
    error = cudaMalloc((void**)&d_C, rowA * colB * sizeof(double));
    
    if(error){
        printf("Error reservando memoria GPU.\n");
        return 0;
    }
    cudaMemset(d_C, 0, rowA * colB * sizeof(double));
    error = cudaMemcpy((void*)d_A, (void*)A, rowA * colA * sizeof(double), cudaMemcpyHostToDevice);
    
    error = cudaMemcpy((void*)d_B, (void*)B, rowB * colB * sizeof(double), cudaMemcpyHostToDevice);
    

    if(error){
        printf("Error copiando memoria a GPU.\n");
        return 0;
    }
    //Grid of 16x16 blocks
    int blocksX = (colB + 15)/16;
    int blocksY = (rowA + 15)/16;
    
    dim3 grid(blocksX, blocksY, 1);
    dim3 block(16, 16, 1);
    start = clock();
    kernel<<<grid, block>>>(d_A, d_B, d_C, rowA, colB, colA);
    end = clock();
    cudaMemcpy(C, d_C, rowA * colB * sizeof(double), cudaMemcpyDeviceToHost);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    return (long int)end-start;
}