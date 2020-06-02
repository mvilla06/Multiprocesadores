#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#define ELEMENTS 100000

int main()
{
    float A[ELEMENTS],
        B[ELEMENTS],
        C[ELEMENTS],
        D[ELEMENTS],
        E[ELEMENTS],
        F[ELEMENTS];
    for (int i = 0; i < ELEMENTS; i++)
    {
        A[i] = 10 + i;
        B[i] = i + 1;
        C[i] = D[i] = E[i] = F[i] = 0;
    }

#pragma omp parallel num_threads(2)
    {
#pragma omp sections
        {
#pragma omp section
            {
                printf("%d\n", omp_get_thread_num());
                for (int i = 0; i < ELEMENTS; i++)
                    C[i] = A[i] + B[i];
                   // printf("%d\n", omp_get_thread_num());
            }
#pragma omp section
            {
                printf("%d\n", omp_get_thread_num());
                for (int i = 0; i < ELEMENTS; i++)
                    D[i] = A[i] - B[i];
               // printf("%d\n", omp_get_thread_num());
            }
            #pragma omp section
        {
            printf("%d\n", omp_get_thread_num());
            for(int i=0; i<ELEMENTS; i++)
                E[i] = A[i]*B[i];
               // printf("%d\n", omp_get_thread_num());
        }
        #pragma omp section
        {
            printf("%d\n", omp_get_thread_num());
            for(int i=0; i<ELEMENTS; i++)
                F[i] = A[i]/B[i];
                
        }
        }
    }

    float *arrays[4];
    arrays[0] = C;
    arrays[1] = D;
    arrays[2] = E;
    arrays[3] = F;
    for (int j = 0; j < 4; j++)
    {
       /* for (int i = 0; i < ELEMENTS; i++)
            printf("%f  ", arrays[j][i]);
        printf("\n");*/
    }

    //printf("%d\n", _OPENMP);

    return 0;
}