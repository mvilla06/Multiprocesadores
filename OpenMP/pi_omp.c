#include <stdio.h>

#include <time.h>

#include <omp.h>

#include <math.h>
#include <string.h>

#define THREAD_NUM 8

long cantidadIntervalos = 1000;

double baseIntervalo;

double acum = 0;

clock_t start, end;

void main()
{
double temps[THREAD_NUM];
   
   
   while (fabs((acum - 3.14159265) * 100000000) > 1)
   {
      baseIntervalo = 1.0 / cantidadIntervalos;
      acum = 0;
      memset(temps, 0, sizeof(double)*THREAD_NUM);
      start = clock();
#pragma omp parallel num_threads(THREAD_NUM)
      {
         double temp = 0;
         float fdx = 0;
         temp = 0;
         double x;
         x = omp_get_thread_num() * baseIntervalo * cantidadIntervalos / omp_get_num_threads();
         x += baseIntervalo / 2; //centrados;
         for (long i = 0; i < cantidadIntervalos / omp_get_num_threads(); i++)
         {

            fdx = 4 / (1 + x * x);

            temp = temp + (fdx * baseIntervalo);

            x = x + baseIntervalo;
         }

         temps[omp_get_thread_num()] = temp;
      }
      acum = 0;
      for (int i = 0; i < THREAD_NUM; i++)
         acum += temps[i];
      end = clock();
      cantidadIntervalos++;
      printf("%f\n", fabs((acum - 3.14159265) * 100000000) );
      printf("%ld\n", cantidadIntervalos);
      printf("Resultado = %20.18lf (%ld)\n", acum, end - start);
   }
}