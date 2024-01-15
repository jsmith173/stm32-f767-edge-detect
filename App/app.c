#include <stdlib.h>

#include "app.h"
#include "conv2d.h"

#define kernel_H 3
#define kernel_W 3

uint8_t* grad;
complex *kernel, *kernel_, *kernel_tmp;
complex *dest;
int dim_kernel[2], dim_src[2];

const complex kernel_scharr[3][3] = {
  {{-3,-3}, {0,-10}, {3,-3}},
  {{-10,0}, {0,0}, {10,0}},
  {{-3,3}, {0,10}, {3,3}}
  };

void setup_kernel()
{
 double r;
 complex c;
 int k, w, h;

 dim_kernel[0] = kernel_H;
 dim_kernel[1] = kernel_W;
 
 h = kernel_H; w = kernel_W;
 for (int i=0; i<h; i++) {
  for (int j=0; j<w; j++) {
   c = kernel_scharr[i][j];
   kernel_[h*i+j] = c;
   kernel[h*i+j] = c;
  }
 }
 mreverse_c(kernel_, kernel_tmp, kernel, dim_kernel);
}

void edge_detect_init()
{
 dest = (complex*)calloc(dim_src[0]*dim_src[1]*sizeof(complex), 1);
 grad = (uint8_t*)calloc(dim_src[0]*dim_src[1]*sizeof(uint8_t), 1);
 kernel = (complex*)calloc(kernel_H*kernel_W*sizeof(complex), 1);
 kernel_ = (complex*)calloc(kernel_H*kernel_W*sizeof(complex), 1);
 kernel_tmp = (complex*)calloc(kernel_H*kernel_W*sizeof(complex), 1);
 setup_kernel();
}

void edge_detect_done()
{
 free(dest);
 free(grad);
 free(kernel);
 free(kernel_);
 free(kernel_tmp);
}

void edge_detect(uint8_t* src)
{
 convolve2d(src, kernel, dest, dim_src, dim_kernel);
 absolute(dest, grad, dim_src);
}
