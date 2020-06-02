#include <omp.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
   int sharedVar = 6;
#pragma omp parallel num_threads(10)
   {
      int privateVar = omp_get_thread_num(); //This data is mine!
      sharedVar = omp_get_thread_num();      //I´ll leave my mark!
      //Add here a pause.
      sleep(1);

      printf("Thread (%d), privateVar = %d, sharedVar= %d \n", omp_get_thread_num(), privateVar, sharedVar);
   }
}
