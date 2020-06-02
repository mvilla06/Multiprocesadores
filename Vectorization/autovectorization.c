#include <stdio.h>
#include <x86intrin.h>
#include <string.h>
#include <time.h>
#pragma warning(disable:4996)
#define TIMES 1000000

void avgNoVect(float *numbers, float *average, int items)
{*average = 0;
float temp = 0;
float num = 0;
size_t x = items;
    for (int i = 0 ; i < x; ++i){
        num = numbers[i];
        temp += num;
    }
        
    *average = temp;
    *average /= items;
}

void avgAutoVect(float *numbers, float *average, int items)
{*average = 0;
float temp = 0;
float num;
int x = items;
    for (int i = 0 ; i < x; i++){
        num = numbers[i];
        temp += num;
    }
    *average = temp;
    *average /= items;
}

void avgVect(float *numbers, float *average, int batches, int size)
{
    *average = 0;
    __m128 A, B, C, D, E;
    D = _mm_set1_ps(0.125);
    E = _mm_set1_ps(0);
    float temp;
    
    for (int i = 0; i < batches; i++)
    {
        A = _mm_load_ps(numbers + i * 8);
        B = _mm_load_ps(numbers + i * 8 + 4);
        C = _mm_hadd_ps(A, B);
        C = _mm_hadd_ps(C, E);
        C = _mm_hadd_ps(C, E);
        
        
        _mm_store_ss(&temp, C);
        
        *average += temp;
        
    }
    *average /=size;
}

int main()
{
    float number, avg1, avg2, avg3;
    int size = 0;
    FILE *file;
    float *numbers;
    int batches;

    time_t start, end;
    time_t start2, end2;
    time_t start3, end3;

    file = fopen("numbers.txt", "r");
    if (file)
    {
        while (fscanf(file, "%f", &number) != EOF)
        {
            
            size++;
        }
        fclose(file);
        batches = (size + 7) / 8;
        numbers = (float *)_mm_malloc(batches * 8 * sizeof(float), 16);
        memset(numbers, batches * 8 * sizeof(float), 0);
        file = fopen("numbers.txt", "r");
        for (int i = 0; i<size; i++)
            fscanf(file, "%f", &numbers[i]);
        
       
    }
    else
    {
        printf("Could not open file.\n");
        return 1;
    }

    fclose(file);

    printf("The file had %d floating point numbers.\n", size);
    printf("Each function was called %d times.\n", TIMES);

    start = clock();
    for (int i = 0; i < TIMES; i++){
        avgNoVect(numbers, &avg1, size);
        
    }
    end = clock();

    start2 = clock();
    for (int i = 0; i < TIMES; i++)
        avgAutoVect(numbers, &avg2, size);

    end2 = clock();

    start3 = clock();
    for (int i = 0; i < TIMES; i++)
        avgVect(numbers, &avg3, batches, size);
    end3 = clock();

    printf("Using scalar operation:\n");
    printf("The average is: %f\n", avg1);
    printf("Time used: (%ld)\n", (long)(end - start));
    printf("Using SSE intrinsics: \n");
    printf("The average is: %f\n", avg3);
    printf("Time used: (%ld)\n", (long)(end3 - start3));
    printf("Using Auto-vectorization: \n");
    printf("The average is: %f\n", avg2);
    printf("Time used: (%ld)\n", (long)(end2 - start2));

    _mm_free(numbers);
    return 0;
}


/*
compiled with the next flags
gcc -o autovect autovectorization.c -march=native -O -ftree-vectorize -msse2 -fopt-info-vec-missed -funsafe-math-optimizations


did not autovectorize average loop.

*/