#include <stdio.h>
#include <string.h>
#include <x86intrin.h>
#include <time.h>
#include <omp.h>

#define VECTOR_SIZE 4


extern long int kernel_wrapper(const double * A, const double * B, double * C, int rowA, int colA, int rowB, int colB);

int main(){
    time_t x1, y1, x2, y2, x3, y3;
    long int avg1 = 0, avg2 = 0, avg3 = 0;
    double * A, * B, * C, * C1, * C2;
    int rowA, colA, rowB, colB, rowC, colC;

    //Leer dimensiones
    printf("Filas de la matriz A: \n");
    scanf("%d", &rowA);
    printf("Columnas de la matriz A: \n");
    scanf("%d", &colA);
    printf("Filas de la matriz B: \n");
    scanf("%d", &rowB);
    printf("Columnas de la matriz B: \n");
    scanf("%d", &colB);

    //Reservar memoria
    int paddingA = (rowA * colA % 4);
    int paddingB = (rowB * colB % 4);
    paddingA = paddingA==0?0:4-paddingA;
    paddingB = paddingB==0?0:4-paddingB;
    
    A = (double *) _mm_malloc(rowA * colA * sizeof(double) + paddingA*sizeof(double), 32);
    B = (double *) _mm_malloc(rowB * colB * sizeof(double) + paddingB*sizeof(double), 32);
    C = (double *) _mm_malloc(rowA * colB * sizeof(double), 32);
    C1 = (double *) _mm_malloc(rowA * colB * sizeof(double), 32);
    C2 = (double *) _mm_malloc(rowA * colB * sizeof(double), 32);

    
    //Verificar memoria
    if(!A || !B || !C || !C1 || !C2){
        printf("Error reservando memoria.\n");
        return 1;
    }

    //Creación de archivos
    FILE * matA, * matB, * matC;
    matA = fopen("matrixA.txt", "r");
    matB = fopen("matrixB.txt", "r");
    matC = fopen("matrixC.txt", "w");

    if(!matA || !matB || !matC){
        printf("Error con los archivos de texto.\n");
        return 1;
    }

    //Lectura de matrices
    for(int i = 0; i < rowA * colA; i++){
        int x = fscanf(matA, "%lf", &A[i]);
        if(x==EOF){
            printf("Datos insuficientes, matriz A.\n");
            goto end;
        }
            
    }

    for(int i = 0; i < rowB; i++){
        for(int j = 0; j < colB; j++){
            int x = fscanf(matB, "%lf", &B[j * rowB + i]); //Transponer matriz B desde la lectura
            if(x==EOF){
                printf("Datos insuficientes, matriz B.\n");
                goto end;
            }
        }   
    }

    //validar dimensiones
    if(colA != rowB){
        printf("Error en las dimensiones de las matrices.\n");
        return 1;
    }

    printf("Corrida\tSerial\tP1\tP2\n");

    for(int x = 0; x<5; x++){
        memset(C, 0,rowA * colB * sizeof(double));
    memset(C1, 0,rowA * colB * sizeof(double));
    memset(C2, 0,rowA * colB * sizeof(double));
    x1 = clock();
    for(int i = 0; i < rowA; i++)
        for(int j = 0; j < colB; j++){
            
            for(int k = 0; k < colA; k++){
               
                C[i * colB + j] += A[i * colA + k] * B[j * rowB + k];
            }
        }
    y1 = clock();

    x2 = 0;
    
    y2 = kernel_wrapper(A, B, C1, rowA, colA, rowB, colB);

    x3 = clock();
    #pragma omp parallel num_threads(4)
    {
        int index = omp_get_thread_num();
        for(int i = 0; i < rowA; i+=omp_get_num_threads())
        
            for(int j = 0; j < colB; j++){
            
            
                
                for(int k=0; k < colA; k++){
                    if(i+index<rowA)
                    C2[(i+index) * colB + j] += A[(i+index) * colA + k] * B[j * rowB + k];
                }
        }
    }
    y3 = clock();
    printf("%d\t%ld\t%ld\t%ld\n", x, y1-x1, y2-x2, y3-x3);
    avg1+=(y1-x1);
    avg2+=(y2-x2);
    avg3+=(y3-x3);
    }
    //Revisar resultados

    printf("Avg\t%ld\t%ld\t%ld\n", avg1/5, avg2/5, avg3/5);
    printf("%% Serial\t-\t%f\t%f\n",  (float)avg2/avg1, (float)avg3/avg1);
    int correct1 = 1;
    for(int i = 0; i < rowA; i++)
        for(int j = 0; j < colB; j++){
            
            if(C[i * colB + j] - C1[i * colB +j] > 0.1)
                correct1 = 0;
        }

    if(correct1){
        printf("Resultados GPU correctos.\n");
    }else{
        printf("Resultados GPU incorrectos.\n");
    }

    int correct2 = 1;
    for(int i = 0; i < rowA; i++)
        for(int j = 0; j < colB; j++){
            
            if(C[i * colB + j] - C2[i * colB +j] > 0.1)
                correct2 = 0;
        }

    if(correct2){
        printf("Resultados OpenMP correctos.\n");
    }else{
        printf("Resultados OpenMP  incorrectos.\n");
    }

    for(int i = 0; i < rowA; i++)
        for(int j = 0; j < colB; j++){
            fprintf(matC, "%.10lf\n", C[i*colB + j]);
        }

    if(avg1<avg2 && avg1<avg3)
        printf("Serial tiene mejor desempeño.\n");
    if(avg2<avg1 && avg2<avg3)
        printf("GPU tiene mejor desempeño.\n");
    if(avg3<avg1 && avg3<avg2)
        printf("OpenMP tiene mejor desempeño.\n");


end:
    _mm_free(A);
    _mm_free(B);
    _mm_free(C);
    _mm_free(C1);
    _mm_free(C2);
    
    fclose(matA);
    fclose(matB);
    fclose(matC);
    
}