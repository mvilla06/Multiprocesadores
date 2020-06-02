#include <stdio.h>
#include <emmintrin.h>
int main() {int array0[4];__m128i a4 = _mm_set_epi32(9, -2, 0, 4);__m128i b4 = _mm_set_epi32(-7, 8, 3, 1);__m128i sum4 = _mm_add_epi32(a4, b4);_mm_storeu_si128((__m128i *)&array0, sum4);for (int i = 0; i <= 3; i++)printf("%d\n", array0[i]);return 0;}
