#include <stdio.h>
#include <omp.h>
#include <time.h>
int main()
{
    int x = 1;
#pragma omp parallel num_threads(4)
    {
        if (omp_get_thread_num() == 0)
        {
            Sleep(2);
            x = 13;
        }
#pragma omp barrier // Race condition has been dealt with.

        printf("Thread (%d): x = %d\n", omp_get_thread_num(), x);
#pragma omp barrier
        if (omp_get_thread_num() == 0)
        { //No race condition.
            printf("Thread (%d): x = %d\n", omp_get_thread_num(), x);
        }
        else
        {
            printf("Thread (%d): x = %d\n", omp_get_thread_num(), x);
        }
    }
    return 0;
}