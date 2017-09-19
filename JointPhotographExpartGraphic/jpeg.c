#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hahuman.h"
#include "dct.h"

#define IMAGE_SIZE 256

int main(){
  int i,n,j,k,u,v;
  unsigned char **raw_image, **raw_image_inversed;
  double **dct_image,  **integral_image_cosine, **integral_dctimage_cosine, **quantized_image, pi = M_PI;
  FILE *fp;

  raw_image = malloc(sizeof(unsigned char*)*IMAGE_SIZE);
  raw_image_inversed = malloc(sizeof(unsigned char*)*IMAGE_SIZE);
  integral_image_cosine = malloc(sizeof(double*)*IMAGE_SIZE);
  integral_dctimage_cosine = malloc(sizeof(double*)*IMAGE_SIZE);
  dct_image = malloc(sizeof(double*)*IMAGE_SIZE);
  quantized_image = malloc(sizeof(double*)*IMAGE_SIZE);

  for(i=0; i<IMAGE_SIZE; i++){
    raw_image[i] = malloc(sizeof(unsigned char)*IMAGE_SIZE);
    raw_image_inversed[i] = malloc(sizeof(unsigned char)*IMAGE_SIZE);
    integral_image_cosine[i] = malloc(sizeof(double)*IMAGE_SIZE);
    integral_dctimage_cosine[i] = malloc(sizeof(double)*IMAGE_SIZE);
    dct_image[i] = malloc(sizeof(double)*IMAGE_SIZE);
    quantized_image[i] = malloc(sizeof(double)*IMAGE_SIZE);

  }

  for(i=0; i<IMAGE_SIZE; i++){
    for(n=0; n<IMAGE_SIZE; n++){
      integral_image_cosine[i][n] = 0;
      integral_dctimage_cosine[i][n] = 0;
    }
  }

  fp = fopen("lenna.raw", "rb");
  for(i=0; i<IMAGE_SIZE; i++){
    for (n=0; n<IMAGE_SIZE; n++){
      fread(&raw_image[i][n], sizeof(unsigned char),1,fp);
    }
  }
  fclose(fp);

  //DCT
  for(i=0; i<32; i++){
    for(n=0; n<32; n++){

      for(u=0; u<8; u++){
        for(v=0; v<8; v++){

          for(j=0; j<8; j++){
            for(k=0; k<8; k++){
               integral_image_cosine[i*8+u][n*8+v] += raw_image[i*8+j][n*8+k] * cos((2.0*j+1.0)*u*pi/16.0) * cos((2.0*k+1.0)*v*pi/16.0);
            }
          }

        }
      }

    }
  }

  for(i=0; i<IMAGE_SIZE; i++){
    for(n=0; n<IMAGE_SIZE; n++){
      u = i%8;
      v = n%8;

      dct_image[i][n] = return_coefficient(u,v)/4.0 * integral_image_cosine[i][n];

    }
  }

  // fp = fopen("dct_image.raw", "wb");
  // for(i=0; i<IMAGE_SIZE; i++){
  //   for (n=0; n<IMAGE_SIZE; n++){
  //     fwrite(&dct_image[i][n], sizeof(double),1,fp);
  //   }
  // }
  // fclose(fp);

  //量子化
  for(i=0; i<32; i++){
    for(n=0; n<32; n++){

      for(u=0; u<8; u++){
        for(v=0; v<8; v++){
          quantized_image[u][v] = dct_image[u][v] * q_table[u][v];
        }
      }

    }
  }




  //逆DCT

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
      raw_image_inversed[i][n] = 1.0/4.0 * integral_dctimage_cosine[i][n];
    }
  }

  fp = fopen("lenna_inversed.raw", "wb");
  for(i=0; i<IMAGE_SIZE; i++){
    for (n=0; n<IMAGE_SIZE; n++){
      fwrite(&raw_image_inversed[i][n], sizeof(unsigned char),1,fp);
    }
  }
  fclose(fp);


  free(raw_image);
  free(integral_image_cosine);
  free(dct_image);

}
