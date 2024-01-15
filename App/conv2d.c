#include <math.h>
#include "conv2d.h"

complex makecomplex(double re, double im)
{
 complex res;

 res.re = re; res.im = im;
 return res;
}

double cabs(complex a)
{
 double res;

 res = sqrt(a.re*a.re+a.im*a.im);
 return res;
}

complex cmul(complex a, complex b)
{
 complex res;

 res.re = a.re * b.re - a.im * b.im;
 res.im = a.im * b.re + a.re * b.im;
 return res;
}

complex cadd(complex a, complex b)
{
 complex res;

 res.re = a.re + b.re;
 res.im = a.im + b.im;
 return res;
}

complex cscale(double c, complex a)
{
 complex res;

 res.re = c*a.re;
 res.im = c*a.im;
 return res;
}

void mreverse_c(complex* src, complex* tmp, complex* dest, int* dim_src)
{
 int h=dim_src[0], w=dim_src[1], u, v;

 for (int i=0; i<h; i++)
  for (int j=0; j<w; j++)
   tmp[h*i+w-1-j] = src[h*i+j];

 for (int i=0; i<h; i++)
  for (int j=0; j<w; j++)
   dest[h*(h-1-i)+j] = tmp[h*i+j];

}

int convolve2d_step(uint8_t* src, complex* kernel, int* dim_src, int* dim_kernel, int m, int n, complex* res)
{
 complex a, tmp, tmp_res;
 int h=dim_kernel[0], w=dim_kernel[1], h_src=dim_src[0], w_src=dim_src[1], u, v;

 tmp_res = makecomplex(0, 0);
 for (int i=0; i<h; i++) {
  for (int j=0; j<w; j++) {
   u = i+m; v = j+n;
   if (u >= h_src || v >= w_src) return 0;
   tmp = cscale(src[h_src*u+v], kernel[h*i+j]);
   tmp_res = cadd(tmp_res, tmp);
  }
 }
 *res = tmp_res;
 return 1;
}

int convolve2d(uint8_t* src, complex* kernel, complex* dest, int* dim_src, int* dim_kernel)
{
 complex a, tmp, res;
 int aa, h_src=dim_src[0], w_src=dim_src[1], h_kernel=dim_kernel[0], w_kernel=dim_kernel[1], m=0, n=0, h, w;

 h = h_src-h_kernel+1; w = w_src-w_kernel+1;
 for (int i=0; i<h; i++) {
  for (int j=0; j<w; j++) {
   if (convolve2d_step(src, kernel, dim_src, dim_kernel, i, j, &res))
	dest[h_src*i+j] = res;
   else
	return 0;
  }
 }
}

int absolute(complex* src, uint8_t* dest, int* dim_src)
{
 complex a, tmp, res;
 int aa, h=dim_src[0], w=dim_src[1], m=0, n=0;
 double r, max_val=0;
 uint8_t u8;

 for (int i=0; i<h; i++) {
  for (int j=0; j<w; j++) {
   r = cabs(src[h*i+j]);
   if (r > max_val) max_val = r;
  }
 }

 for (int i=0; i<h; i++) {
  for (int j=0; j<w; j++) {
   r = cabs(src[h*i+j]);
   r = r/max_val*255.0;
   u8 = r;
   dest[h*i+j] = u8;
  }
 }

 return 1;
}


