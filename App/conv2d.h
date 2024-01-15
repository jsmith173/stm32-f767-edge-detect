#ifndef conv2dH
#define conv2dH

#include <stdint.h>

typedef struct {
 double re, im;
} complex;

int convolve2d(uint8_t* src, complex* kernel, complex* dest, int* dim_src, int* dim_kernel);
int absolute(complex* src, uint8_t* dest, int* dim_src);
complex makecomplex(double re, double im);
void mreverse_c(complex* src, complex* tmp, complex* dest, int* dim_src);

#endif

