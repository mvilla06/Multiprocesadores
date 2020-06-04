app: cpu.o gpu.o

	nvcc -o app cpu.o gpu.o -Xcompiler -fopenmp

cpu.o: matrixMultiplication.c 

	gcc -I /usr/local/cuda-10.1/include -c -o cpu.o matrixMultiplication.c -fopenmp

gpu.o: kernel.cu 

	nvcc -c -o gpu.o kernel.cu --cudart static 