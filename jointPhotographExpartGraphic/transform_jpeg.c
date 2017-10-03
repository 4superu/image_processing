#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hahuman.h"
#include "jpeg_transform.h"

#define IMAGE_SIZE 256

int main(){
  int i,n,j,k,u,v,block_number;
  unsigned char **raw_image;
  double **dct_image, **scaned_quantized_image, **scaned_quantized_image_arry,  **integral_image_cosine, **integral_dctimage_cosine, **quantized_image, **encoded_image, pi = M_PI;
  FILE *fp;

  raw_image = malloc(sizeof(unsigned char*)*IMAGE_SIZE);
  integral_image_cosine = malloc(sizeof(double*)*IMAGE_SIZE);
  dct_image = malloc(sizeof(double*)*IMAGE_SIZE);
  scaned_quantized_image = malloc(sizeof(double*)*IMAGE_SIZE);
  scaned_quantized_image_arry = malloc(sizeof(double*)*1024);
  quantized_image = malloc(sizeof(double*)*IMAGE_SIZE);
  encoded_image = malloc(sizeof(double*)*IMAGE_SIZE);

  for(i=0; i<IMAGE_SIZE; i++){
    raw_image[i] = malloc(sizeof(unsigned char)*IMAGE_SIZE);
    integral_image_cosine[i] = malloc(sizeof(double)*IMAGE_SIZE);
    dct_image[i] = malloc(sizeof(double)*IMAGE_SIZE);
    scaned_quantized_image[i] = malloc(sizeof(double)*IMAGE_SIZE);
    quantized_image[i] = malloc(sizeof(double)*IMAGE_SIZE);
    encoded_image[i] = malloc(sizeof(double)*IMAGE_SIZE);
  }

  for(i=0; i<1024; i++){
    scaned_quantized_image_arry[i] = malloc(sizeof(double)*64);
  }

  for(i=0; i<IMAGE_SIZE; i++){
    for(n=0; n<IMAGE_SIZE; n++){
      integral_image_cosine[i][n] = 0;
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

  fp = fopen("dct_image.raw", "wb");
  for(i=0; i<IMAGE_SIZE; i++){
    for (n=0; n<IMAGE_SIZE; n++){
      fwrite(&dct_image[i][n], sizeof(double),1,fp);
    }
  }
  fclose(fp);

  //量子化
  for(i=0; i<32; i++){
    for(n=0; n<32; n++){

      for(u=0; u<8; u++){
        for(v=0; v<8; v++){
          quantized_image[i*8+u][n*8+v] = dct_image[i*8+u][n*8+v] / q_table[u][v];
        }
      }

    }
  }

  block_number = 0;
  for(n=0; n<32; n++){
    for(i=0; i<32; i++){
      scaned_quantized_image_arry[block_number][0] = quantized_image[i*8+0][n*8+0];
      scaned_quantized_image_arry[block_number][1] = quantized_image[i*8+1][n*8+0];
      scaned_quantized_image_arry[block_number][2] = quantized_image[i*8+0][n*8+1];
      scaned_quantized_image_arry[block_number][3] = quantized_image[i*8+0][n*8+2];
      scaned_quantized_image_arry[block_number][4] = quantized_image[i*8+1][n*8+1];
      scaned_quantized_image_arry[block_number][5] = quantized_image[i*8+2][n*8+0];
      scaned_quantized_image_arry[block_number][6] = quantized_image[i*8+3][n*8+0];
      scaned_quantized_image_arry[block_number][7] = quantized_image[i*8+2][n*8+1];

      scaned_quantized_image_arry[block_number][8] = quantized_image[i*8+1][n*8+2];
      scaned_quantized_image_arry[block_number][9] = quantized_image[i*8+0][n*8+3];
      scaned_quantized_image_arry[block_number][10] = quantized_image[i*8+0][n*8+4];
      scaned_quantized_image_arry[block_number][11] = quantized_image[i*8+1][n*8+3];
      scaned_quantized_image_arry[block_number][12] = quantized_image[i*8+2][n*8+2];
      scaned_quantized_image_arry[block_number][13] = quantized_image[i*8+3][n*8+1];
      scaned_quantized_image_arry[block_number][14] = quantized_image[i*8+4][n*8+0];
      scaned_quantized_image_arry[block_number][15] = quantized_image[i*8+5][n*8+0];

      scaned_quantized_image_arry[block_number][16] = quantized_image[i*8+4][n*8+1];
      scaned_quantized_image_arry[block_number][17] = quantized_image[i*8+3][n*8+2];
      scaned_quantized_image_arry[block_number][18] = quantized_image[i*8+2][n*8+3];
      scaned_quantized_image_arry[block_number][19] = quantized_image[i*8+1][n*8+4];
      scaned_quantized_image_arry[block_number][20] = quantized_image[i*8+0][n*8+5];
      scaned_quantized_image_arry[block_number][21] = quantized_image[i*8+0][n*8+6];
      scaned_quantized_image_arry[block_number][22] = quantized_image[i*8+1][n*8+5];
      scaned_quantized_image_arry[block_number][23] = quantized_image[i*8+2][n*8+4];

      scaned_quantized_image_arry[block_number][24] = quantized_image[i*8+3][n*8+3];
      scaned_quantized_image_arry[block_number][25] = quantized_image[i*8+4][n*8+2];
      scaned_quantized_image_arry[block_number][26] = quantized_image[i*8+5][n*8+1];
      scaned_quantized_image_arry[block_number][27] = quantized_image[i*8+6][n*8+0];
      scaned_quantized_image_arry[block_number][28] = quantized_image[i*8+7][n*8+0];
      scaned_quantized_image_arry[block_number][29] = quantized_image[i*8+6][n*8+1];
      scaned_quantized_image_arry[block_number][30] = quantized_image[i*8+5][n*8+2];
      scaned_quantized_image_arry[block_number][31] = quantized_image[i*8+4][n*8+3];

      scaned_quantized_image_arry[block_number][32] = quantized_image[i*8+3][n*8+4];
      scaned_quantized_image_arry[block_number][33] = quantized_image[i*8+2][n*8+5];
      scaned_quantized_image_arry[block_number][34] = quantized_image[i*8+1][n*8+6];
      scaned_quantized_image_arry[block_number][35] = quantized_image[i*8+0][n*8+7];
      scaned_quantized_image_arry[block_number][36] = quantized_image[i*8+1][n*8+7];
      scaned_quantized_image_arry[block_number][37] = quantized_image[i*8+2][n*8+6];
      scaned_quantized_image_arry[block_number][38] = quantized_image[i*8+3][n*8+5];
      scaned_quantized_image_arry[block_number][39] = quantized_image[i*8+4][n*8+4];

      scaned_quantized_image_arry[block_number][40] = quantized_image[i*8+5][n*8+3];
      scaned_quantized_image_arry[block_number][41] = quantized_image[i*8+6][n*8+2];
      scaned_quantized_image_arry[block_number][42] = quantized_image[i*8+7][n*8+1];
      scaned_quantized_image_arry[block_number][43] = quantized_image[i*8+7][n*8+2];
      scaned_quantized_image_arry[block_number][44] = quantized_image[i*8+6][n*8+3];
      scaned_quantized_image_arry[block_number][45] = quantized_image[i*8+5][n*8+4];
      scaned_quantized_image_arry[block_number][46] = quantized_image[i*8+4][n*8+5];
      scaned_quantized_image_arry[block_number][47] = quantized_image[i*8+3][n*8+6];

      scaned_quantized_image_arry[block_number][48] = quantized_image[i*8+2][n*8+7];
      scaned_quantized_image_arry[block_number][49] = quantized_image[i*8+3][n*8+7];
      scaned_quantized_image_arry[block_number][50] = quantized_image[i*8+4][n*8+6];
      scaned_quantized_image_arry[block_number][51] = quantized_image[i*8+5][n*8+5];
      scaned_quantized_image_arry[block_number][52] = quantized_image[i*8+6][n*8+4];
      scaned_quantized_image_arry[block_number][53] = quantized_image[i*8+7][n*8+3];
      scaned_quantized_image_arry[block_number][54] = quantized_image[i*8+7][n*8+4];
      scaned_quantized_image_arry[block_number][55] = quantized_image[i*8+6][n*8+5];

      scaned_quantized_image_arry[block_number][56] = quantized_image[i*8+5][n*8+6];
      scaned_quantized_image_arry[block_number][57] = quantized_image[i*8+4][n*8+7];
      scaned_quantized_image_arry[block_number][58] = quantized_image[i*8+5][n*8+7];
      scaned_quantized_image_arry[block_number][59] = quantized_image[i*8+6][n*8+6];
      scaned_quantized_image_arry[block_number][60] = quantized_image[i*8+7][n*8+5];
      scaned_quantized_image_arry[block_number][61] = quantized_image[i*8+7][n*8+6];
      scaned_quantized_image_arry[block_number][62] = quantized_image[i*8+6][n*8+7];
      scaned_quantized_image_arry[block_number][63] = quantized_image[i*8+7][n*8+7];
      block_number++;
      printf("%lf\n",quantized_image[i*8+0][n*8+0]);
    }
  }

  int element_number = 0;
  char hahumancode;
  for(block_number=0; block_number<32*32; block_number++){
    for(element_number=0; element_number<64; element_number++){

      if(element_number == 0){
        dc_encode_hahuman
      }
      else{
        // ac_encode_hahuman(block_number, element_number, scaned_quantized_image_arry);
      }

    }
  }




  free(raw_image);
  free(dct_image);
  free(scaned_quantized_image);
  free(integral_image_cosine);
  free(quantized_image);

}
