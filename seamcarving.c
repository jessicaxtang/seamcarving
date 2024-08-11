#if !defined(SEAMCARVING_H)
#define SEAMCARVING_H

/* PROJECT 2 */
// #include "c_img.c"
#include "c_img.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void calc_energy(struct rgb_img *im, struct rgb_img **grad) {
  create_img(grad, im->height, im->width);
  
  for (int y = 0; y < im->height ; y++) {
    for (int x = 0; x < im->width ; x++) {
      int left = x - 1;
      int right = x + 1;
      int above = y - 1;
      int below = y + 1;

      if (x == 0){
        left = im->width - 1;
      }
      if (x == im->width - 1){
        right = 0;
      }
      if (y == 0){
        above = im->height - 1;
      }
      if (y == im->height - 1){
        below = 0;
      }
        
      int rx = get_pixel(im, y, left, 0) - get_pixel(im, y, right, 0);
      int gx = get_pixel(im, y, left, 1) - get_pixel(im, y, right, 1);
      int bx = get_pixel(im, y, left, 2) - get_pixel(im, y, right, 2);

      int ry = get_pixel(im, above, x, 0) - get_pixel(im, below, x, 0);
      int gy = get_pixel(im, above, x, 1) - get_pixel(im, below, x, 1);
      int by = get_pixel(im, above, x, 2) - get_pixel(im, below, x, 2);

      int delta_x = (rx * rx) + (gx * gx) + (bx * bx);
      int delta_y = (ry * ry) + (gy * gy) + (by * by);
      int pix_grad = sqrt(delta_x + delta_y) / 10;

      set_pixel(*grad, y, x, pix_grad, pix_grad, pix_grad);
      }
    }
  }

double minimum(double left, double mid, double right){
  return fmin(fmin(left, mid), fmin(mid, right)); 
}

void dynamic_seam(struct rgb_img *grad, double **best_arr){ 
  *best_arr = (double *)malloc((grad->width)*(grad->height)*sizeof(double)); 

  // y == 0
  for (int x = 0; x < grad->width; x ++){
      (*best_arr)[x] = (double)get_pixel(grad, 0, x, 0); 
  } 
  
  for (int y = 1; y < grad->height; y++){
    for (int x = 0; x < grad->width; x ++){
      double previous;
      if (x == 0){
        previous = minimum((*best_arr)[(y-1)*grad->width+x]+1, (*best_arr)[(y-1)*grad->width+x], (*best_arr)[(y-1)*grad->width+x+1]);
        (*best_arr)[y*grad->width+x] = previous + (double)get_pixel(grad, y, x, 0); 
        
      }
      else if (x == grad->width - 1){
        previous = minimum((*best_arr)[(y-1)*grad->width+x-1], (*best_arr)[(y-1)*grad->width+x], (*best_arr)[(y-1)*grad->width+x-1]+1);
      }
      else{
         previous = minimum((*best_arr)[(y-1)*grad->width+x-1], (*best_arr)[(y-1)*grad->width+x], (*best_arr)[(y-1)*grad->width+x+1]);
      }
      (*best_arr)[y*grad->width+x] = previous + (double)get_pixel(grad, y, x, 0); 
    }
  }
}

int minimum_index(double left, double mid, double right){
  double cur_min = left;
  int min_ind = -1;
  if (mid < cur_min){
    cur_min = mid;
    min_ind = 0;
  }
  else if (right < cur_min){
    cur_min = right; 
    min_ind = 1;
  }

  return min_ind; 
}

void recover_path(double *best_arr, int height, int width, int **path){
  *path = (int *)malloc(height*sizeof(int));
  int cur_min = best_arr[(height - 1) * (width)]; // first entry of last row
  int cur_x = 0;
  for (int i = 0; i < width; i++){
    if (best_arr[(height - 1) * width + i] < cur_min){
      cur_min = best_arr[(height - 1) * width + i];
      cur_x = i; 
    } 
  }
  (*path)[height-1] = cur_x; 
  
  for (int y = height - 1; y > 0; y--){
    if (cur_x == 0){
      (*path)[y-1] = cur_x + minimum_index(best_arr[(y-1)*width+cur_x]+1, best_arr[(y-1)*width+cur_x], best_arr[(y-1)*width+cur_x+1]);
    }
    else if (cur_x == width-1){
      (*path)[y-1] = cur_x + minimum_index(best_arr[(y-1)*width+cur_x-1], best_arr[(y-1)*width+cur_x], best_arr[(y-1)*width+cur_x]+1);
    }
    else {
      (*path)[y-1] = cur_x + minimum_index(best_arr[(y-1)*width+cur_x-1], best_arr[(y-1)*width+cur_x], best_arr[(y-1)*width+cur_x+1]);
    }
    cur_x = (*path)[y-1]; 
  }
}

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){
  create_img(dest, src->height, src->width-1);
  for (int y = 0; y < src->height; y++){
    int x = 0;
    while(x != path[y]){
      int r = get_pixel(src, y, x, 0);
      int g = get_pixel(src, y, x, 1);
      int b = get_pixel(src, y, x, 2);
      set_pixel(*dest, y, x, r, g, b);
      x++;
    }
    x += 1;
    while(x <= src->width){
      int r = get_pixel(src, y, x, 0);
      int g = get_pixel(src, y, x, 1);
      int b = get_pixel(src, y, x, 2);
      set_pixel(*dest, y, x - 1, r, g, b);
      x++;
    }
  }
}

#endif