#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "hahuman.h"

#define IMAGE_SIZE 256

//DCT処理

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

void discreteCosineTransform(unsigned char **raw_image, double **dct_image){

  int i,n,j,k,u,v;
  double integral_image_cosine;

  for(i=0; i<32; i++){
    for(n=0; n<32; n++){

      for(u=0; u<8; u++){
        for(v=0; v<8; v++){

          integral_image_cosine = 0;
          for(j=0; j<8; j++){
            for(k=0; k<8; k++){
               integral_image_cosine += raw_image[i*8+j][n*8+k] * cos((2.0*j+1.0)*u*M_PI/16.0) * cos((2.0*k+1.0)*v*M_PI/16.0);
            }
          }

          dct_image[i*8+u][n*8+v] = return_coefficient(u,v)/4.0 * integral_image_cosine;

        }
      }

    }
  }
}

//量子化

int integer_by_threshould(double quantized_image_element){

  if(0 <= quantized_image_element){
    return (int)(quantized_image_element + 0.5);
  }
  else{
    return (int)(quantized_image_element - 0.5);
  }
}

void quantization(double **dct_image, int **quantized_image){
  int i,n,u,v;

  for(i=0; i<32; i++){
    for(n=0; n<32; n++){

      for(u=0; u<8; u++){
        for(v=0; v<8; v++){
          quantized_image[i*8+u][n*8+v] = integer_by_threshould(dct_image[i*8+u][n*8+v] / q_table[u][v]);
        }
      }

    }
  }
}

void scan_zigzag(int **scaned_quantized_image_arry, int **quantized_image){
  int i,n,block_number=0;

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
    }
  }
}

//符号化

int dc_different_value(int **scaned_quantized_image_arry, int block_number){
  if(block_number == 0){
    return scaned_quantized_image_arry[block_number][0];
  }
  else{
    return scaned_quantized_image_arry[block_number][0] - scaned_quantized_image_arry[block_number-1][0];
  }
}

int select_dc_group(int **scaned_quantized_image_arry, int block_number){

  int difference_dc_element;

  difference_dc_element = dc_different_value(scaned_quantized_image_arry, block_number);

  if(difference_dc_element == 0){
    return 0;
  }
  else{
    return (int)floor(log2(abs(difference_dc_element))+1.0);
  }
}

unsigned long binary_conversion(int decimal_number){
  unsigned long binary=0, base=1;
  while(decimal_number > 0){
    binary = binary + ( decimal_number % 2 ) * base;
    decimal_number = decimal_number / 2;
    base = base * 10;
  }
  return binary;

}

void binary_conversion_negative(int decimal_number, char *binary_code, int *bit_number){
  *bit_number = 0;
  while(abs(decimal_number) > 0){
    if(abs(decimal_number)%2 == 1){
      binary_code[*bit_number] = 0;
    }
    else{
      binary_code[*bit_number] = 1;
    }
    decimal_number = decimal_number/2;
    *bit_number = *bit_number + 1;
  }
}

int select_ac_group(int ac_coefficient){
  ac_coefficient = abs(ac_coefficient);
  if(abs(ac_coefficient) == 0){
    return 0;
  }
  else{
    return (int)floor(log2(ac_coefficient))+1;
  }
}
