#include <stdio.h>
#include <omp.h>
int main()
{
    int rT = 8850; //Requested Threads
    int aT = 0;    //Actual Threads
    do
    {
        rT += 1;
        omp_set_num_threads(rT);
#pragma omp parallel
        {
            if (omp_get_thread_num() == 0)
            {       
                aT = omp_get_num_threads();
                                    printf("Master Control Program gave me %d threads. He's a nice program.\n", aT);
            }   
            int a = 50 + 100; //Just some work...
        }
    } while (rT == aT);
    printf("\nI take it back! Master Control Program gave me %d thread. He's EVIL!\n", aT);
    return 0;
}