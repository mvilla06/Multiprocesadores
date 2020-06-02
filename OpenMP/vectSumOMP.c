#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <time.h>

#if defined(DOUBLE)
#define NUM_THREADS 8
#elif defined(SYSTEM)
#define NUM_THREADS 4
#else
#define NUM_THREADS 9910
#endif

int main(){
    float A[16];
    float B[16];
    float C[16];
    time_t start, end;

    for (int i = 0; i<16; i++)
        A[i] = B[i] = i;
    
    memset(C, 0, 16*sizeof(float));
    int max = omp_get_max_threads();
    omp_set_num_threads(max);
    start = clock();
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        if(omp_get_thread_num()==0)
            printf("%d\n", omp_get_num_threads());
        /*if(omp_get_thread_num()<16)
            C[omp_get_thread_num()] = A[omp_get_thread_num()]+B[omp_get_thread_num()];
        #ifdef DOUBLE
            C[omp_get_thread_num()+8] = A[omp_get_thread_num()+8]+B[omp_get_thread_num()+8];
        #elif defined(SYSTEM)
            C[omp_get_thread_num()+8] = A[omp_get_thread_num()+8]+B[omp_get_thread_num()+8];
            C[omp_get_thread_num()+4] = A[omp_get_thread_num()+4]+B[omp_get_thread_num()+4];
            C[omp_get_thread_num()+12] = A[omp_get_thread_num()+12]+B[omp_get_thread_num()+12];
        #endif*/

    }
    end = clock();
    for (int i = 0; i<16; i++)
        printf("%f\n", C[i]);
    printf("%ld\n", (long) (end-start));
}