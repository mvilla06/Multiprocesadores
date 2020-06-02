#include <nmmintrin.h>
#include <stdio.h>


void lp(float * x, float *y, int n){
	__m128 div, v1, v2, avg;
	div = _mm_set1_ps(2.0);
	for(int i=0; i<n/8; i++){
		v1 = _mm_load_ps(x+i*8);
		v2 = _mm_load_ps(x+4+i*8);
		avg = _mm_hadd_ps(v1, v2);
		avg = _mm_div_ps(avg, div);
		_mm_store_ps(y+i*4, avg);
	}
}

int main(){
float* a;
float* b;
int x= 8;
a = (float*)malloc(8*sizeof(float));
b = (float*)malloc(4*sizeof(float));
for(int i=0;i<8;i++)
	a[i]=i;
lp(a, b, x);
for(int i=0;i<4; i++)
	printf("%f\n", b[i]);
return 0;
}
