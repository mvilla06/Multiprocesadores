#include <stdio.h>
#include <omp.h>
int main(){
    omp_set_num_threads(8);
    #pragma omp parallel num_threads(4)
    {
        printf("Hello OMP World!...\n");
    }
}