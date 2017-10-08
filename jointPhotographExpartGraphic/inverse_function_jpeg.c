#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hahuman.h"

#define IMAGE_SIZE 256

double return_coefficient(int u, int v){
  switch (u) {
    case 0:
    switch (v) {
      case 0:
      return 1.0/2.0;

      default:
      return 1.0/sqrt(2.0);
    }

    default:
    switch (v) {
      case 0:
      return 1.0/sqrt(2.0);

      default:
      return 1.0;
    }
  }
}


void inverseDiscreteCosineTransform(unsigned char **raw_image, double **inverse_dct_image){

  int i,n,j,k,u,v;
  double integral_dctimage_cosine;

  for(i=0; i<32; i++){
    for(n=0; n<32; n++){

      for(j=0; j<8; j++){
        for(k=0; k<8; k++){

          integral_dctimage_cosine = 0;
          for(u=0; u<8; u++){
            for(v=0; v<8; v++){
              integral_dctimage_cosine += return_coefficient(u,v) * inverse_dct_image[i*8+u][n*8+v] * cos((2.0*j+1.0)*u*M_PI/16.0) * cos((2.0*k+1.0)*v*M_PI/16.0);
            }
          }

          raw_image[i*8+j][n*8+k] = (unsigned char)1.0/4.0 * integral_dctimage_cosine;

        }
      }

    }
  }
}



void inverse_quantization(int **inverse_encoded_image, double **inverse_quantized_image){
  int i,n,u,v;

  for(i=0; i<32; i++){
    for(n=0; n<32; n++){
      for(u=0; u<8; u++){
        for(v=0; v<8; v++){
          inverse_quantized_image[i*8+u][n*8+v] = q_table[u][v] * inverse_encoded_image[i*8+u][n*8+v];
        }
      }
      inverse_quantized_image[i][n] = q_table[i][n] * inverse_encoded_image[i][n];
    }
  }
}
