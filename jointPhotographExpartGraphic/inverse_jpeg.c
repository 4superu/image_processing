#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hahuman.h"
#include "dct.h"

int main(){

  int i,n,j,k,u,v;
  // compression
  unsigned char **jpeg_image;
  double **integral_dctimage_cosine, pi = M_PI;
  FILE *fp;

  jpeg_image = malloc(sizeof(unsigned char*)*IMAGE_SIZE);
  integral_dctimage_cosine = malloc(sizeof(double*)*IMAGE_SIZE);

  for(i=0; i<IMAGE_SIZE; i++){
    jpeg_image[i] = malloc(sizeof(unsigned char)*IMAGE_SIZE);
    integral_dctimage_cosine[i] = malloc(sizeof(double)*IMAGE_SIZE);
    dct_image[i] = malloc(sizeof(double)*IMAGE_SIZE);
  }

  for(i=0; i<IMAGE_SIZE; i++){
    for(n=0; n<IMAGE_SIZE; n++){
      integral_dctimage_cosine[i][n] = 0;
    }
  }

  for(i=0; i<32; i++){
    for(n=0; n<32; n++){

      for(j=0; j<8; j++){
        for(k=0; k<8; k++){

          for(u=0; u<8; u++){
            for(v=0; v<8; v++){
              integral_dctimage_cosine[i*8+j][n*8+k] += return_coefficient(u,v) * dct_image[i*8+u][n*8+v] * cos((2.0*j+1.0)*u*pi/16.0) * cos((2.0*k+1.0)*v*pi/16.0);
            }
          }

        }
      }

    }
  }

  for(i=0; i<IMAGE_SIZE; i++){
    for(n=0; n<IMAGE_SIZE; n++){
      jpeg_image[i][n] = 1.0/4.0 * integral_dctimage_cosine[i][n];
    }
  }

  fp = fopen("lenna_inversed.raw", "wb");
  for(i=0; i<IMAGE_SIZE; i++){
    for (n=0; n<IMAGE_SIZE; n++){
      fwrite(&jpeg_image[i][n], sizeof(unsigned char),1,fp);
    }
  }
  fclose(fp);

}
