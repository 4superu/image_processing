#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "hahuman.h"

#define IMAGE_SIZE 256

//transform側
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
          quantized_image[i*8+u][n*8+v] = integer_by_threshould(dct_image[i*8+u][n*8+v] / (double)q_table[u][v]);
        }
      }
    }
  }
}

void scan_zigzag(int **scaned_quantized_image_arry, int **quantized_image){
  int i,n,block_number=0;

  for(i=0; i<32; i++){
    for(n=0; n<32; n++){

      scaned_quantized_image_arry[block_number][0] = quantized_image[i*8+0][n*8+0];
      scaned_quantized_image_arry[block_number][1] = quantized_image[i*8+0][n*8+1];
      scaned_quantized_image_arry[block_number][2] = quantized_image[i*8+1][n*8+0];
      scaned_quantized_image_arry[block_number][3] = quantized_image[i*8+2][n*8+0];
      scaned_quantized_image_arry[block_number][4] = quantized_image[i*8+1][n*8+1];
      scaned_quantized_image_arry[block_number][5] = quantized_image[i*8+0][n*8+2];
      scaned_quantized_image_arry[block_number][6] = quantized_image[i*8+0][n*8+3];
      scaned_quantized_image_arry[block_number][7] = quantized_image[i*8+1][n*8+2];

      scaned_quantized_image_arry[block_number][8] = quantized_image[i*8+2][n*8+1];
      scaned_quantized_image_arry[block_number][9] = quantized_image[i*8+3][n*8+0];
      scaned_quantized_image_arry[block_number][10] = quantized_image[i*8+4][n*8+0];
      scaned_quantized_image_arry[block_number][11] = quantized_image[i*8+3][n*8+1];
      scaned_quantized_image_arry[block_number][12] = quantized_image[i*8+2][n*8+2];
      scaned_quantized_image_arry[block_number][13] = quantized_image[i*8+1][n*8+3];
      scaned_quantized_image_arry[block_number][14] = quantized_image[i*8+0][n*8+4];
      scaned_quantized_image_arry[block_number][15] = quantized_image[i*8+0][n*8+5];

      scaned_quantized_image_arry[block_number][16] = quantized_image[i*8+1][n*8+4];
      scaned_quantized_image_arry[block_number][17] = quantized_image[i*8+2][n*8+3];
      scaned_quantized_image_arry[block_number][18] = quantized_image[i*8+3][n*8+2];
      scaned_quantized_image_arry[block_number][19] = quantized_image[i*8+4][n*8+1];
      scaned_quantized_image_arry[block_number][20] = quantized_image[i*8+5][n*8+0];
      scaned_quantized_image_arry[block_number][21] = quantized_image[i*8+6][n*8+0];
      scaned_quantized_image_arry[block_number][22] = quantized_image[i*8+5][n*8+1];
      scaned_quantized_image_arry[block_number][23] = quantized_image[i*8+4][n*8+2];

      scaned_quantized_image_arry[block_number][24] = quantized_image[i*8+3][n*8+3];
      scaned_quantized_image_arry[block_number][25] = quantized_image[i*8+2][n*8+4];
      scaned_quantized_image_arry[block_number][26] = quantized_image[i*8+1][n*8+5];
      scaned_quantized_image_arry[block_number][27] = quantized_image[i*8+0][n*8+6];
      scaned_quantized_image_arry[block_number][28] = quantized_image[i*8+0][n*8+7];
      scaned_quantized_image_arry[block_number][29] = quantized_image[i*8+1][n*8+6];
      scaned_quantized_image_arry[block_number][30] = quantized_image[i*8+2][n*8+5];
      scaned_quantized_image_arry[block_number][31] = quantized_image[i*8+3][n*8+4];

      scaned_quantized_image_arry[block_number][32] = quantized_image[i*8+4][n*8+3];
      scaned_quantized_image_arry[block_number][33] = quantized_image[i*8+5][n*8+2];
      scaned_quantized_image_arry[block_number][34] = quantized_image[i*8+6][n*8+1];
      scaned_quantized_image_arry[block_number][35] = quantized_image[i*8+7][n*8+0];
      scaned_quantized_image_arry[block_number][36] = quantized_image[i*8+7][n*8+1];
      scaned_quantized_image_arry[block_number][37] = quantized_image[i*8+6][n*8+2];
      scaned_quantized_image_arry[block_number][38] = quantized_image[i*8+5][n*8+3];
      scaned_quantized_image_arry[block_number][39] = quantized_image[i*8+4][n*8+4];

      scaned_quantized_image_arry[block_number][40] = quantized_image[i*8+3][n*8+5];
      scaned_quantized_image_arry[block_number][41] = quantized_image[i*8+2][n*8+6];
      scaned_quantized_image_arry[block_number][42] = quantized_image[i*8+1][n*8+7];
      scaned_quantized_image_arry[block_number][43] = quantized_image[i*8+2][n*8+7];
      scaned_quantized_image_arry[block_number][44] = quantized_image[i*8+3][n*8+6];
      scaned_quantized_image_arry[block_number][45] = quantized_image[i*8+4][n*8+5];
      scaned_quantized_image_arry[block_number][46] = quantized_image[i*8+5][n*8+4];
      scaned_quantized_image_arry[block_number][47] = quantized_image[i*8+6][n*8+3];

      scaned_quantized_image_arry[block_number][48] = quantized_image[i*8+7][n*8+2];
      scaned_quantized_image_arry[block_number][49] = quantized_image[i*8+7][n*8+3];
      scaned_quantized_image_arry[block_number][50] = quantized_image[i*8+6][n*8+4];
      scaned_quantized_image_arry[block_number][51] = quantized_image[i*8+5][n*8+5];
      scaned_quantized_image_arry[block_number][52] = quantized_image[i*8+4][n*8+6];
      scaned_quantized_image_arry[block_number][53] = quantized_image[i*8+3][n*8+7];
      scaned_quantized_image_arry[block_number][54] = quantized_image[i*8+4][n*8+7];
      scaned_quantized_image_arry[block_number][55] = quantized_image[i*8+5][n*8+6];

      scaned_quantized_image_arry[block_number][56] = quantized_image[i*8+6][n*8+5];
      scaned_quantized_image_arry[block_number][57] = quantized_image[i*8+7][n*8+4];
      scaned_quantized_image_arry[block_number][58] = quantized_image[i*8+7][n*8+5];
      scaned_quantized_image_arry[block_number][59] = quantized_image[i*8+6][n*8+6];
      scaned_quantized_image_arry[block_number][60] = quantized_image[i*8+5][n*8+7];
      scaned_quantized_image_arry[block_number][61] = quantized_image[i*8+6][n*8+7];
      scaned_quantized_image_arry[block_number][62] = quantized_image[i*8+7][n*8+6];
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

void dc_group_binary(int **scaned_quantized_image_arry, int block_number, long *encode_data_number, int *encode_data_arry) {
  int i,difference_dc_element, dc_group;

  difference_dc_element = dc_different_value(scaned_quantized_image_arry, block_number);
  if(difference_dc_element == 0){
    dc_group = 0;
  }
  else{
    dc_group = (int)floor(log2(abs(difference_dc_element))+1.0);
  }
  for(i=0; i<dc_length_table[dc_group]; i++){
    encode_data_arry[*encode_data_number] = dc_code_table[dc_group][i];
    *encode_data_number += 1;

  }
}

void binary_conversion(int decimal_number, int *encode_data_arry, long *encode_data_number){

  int bit_number = 0, binary_code[12] = {0};

  while(abs(decimal_number) >0){
    //値が正の時
    if(decimal_number > 0){
      if(decimal_number%2 == 1){
        binary_code[bit_number] = 1;
      }
      else{
        binary_code[bit_number] = 0;
      }
    }
    else{
      if(abs(decimal_number)%2 == 1){
        binary_code[bit_number] = 0;
      }
      else{
        binary_code[bit_number] = 1;
      }
    }
    decimal_number = decimal_number/2;
    bit_number += 1;
  }

  while(bit_number > 0){
    encode_data_arry[*encode_data_number] = binary_code[bit_number-1];
    bit_number--;
    *encode_data_number += 1;
  }
}

int select_ac_group(int ac_coefficient){
  ac_coefficient = abs(ac_coefficient);
  if(ac_coefficient == 0){
    return 0;
  }
  else{
    return (int)floor(log2(ac_coefficient)+1.0);
  }
}
void ac_group_binary(int *encode_data_arry, long *encode_data_number, int ac_group, int *ac_zero_run){
  int i;

  if(ac_group != 0){
    for(i=0; i<ac_length_table[*ac_zero_run*11+ac_group]; i++){
      encode_data_arry[*encode_data_number] = ac_code_table[*ac_zero_run*11 + ac_group][i];
      *encode_data_number += 1;

    }
    *ac_zero_run = 0;
  }
}


void add_zero_run_code(int *encode_data_arry, long *encode_data_number, int *ac_zero_run){
  int i;

  while(*ac_zero_run >= 15){
    for(i=0; i<ac_length_table[165]; i++){
      encode_data_arry[*encode_data_number] = ac_code_table[165][i];
      *encode_data_number += 1;
    }
    *ac_zero_run -= 15;
  }

}

void add_block_end_binary(int *encode_data_arry, long *encode_data_number){
  int i;
  for(i=0; i<4; i++){
    encode_data_arry[*encode_data_number] = (i+1)%2;
    *encode_data_number += 1;
  }
}

void compression_to_raw(unsigned char *raw_encode_data, long *encode_data_number, int *encode_data_arry){
  int i;

  for(i=0; i<*encode_data_number; i++){
    if(encode_data_arry[i] == 1){
      raw_encode_data[i/8] += pow(2,(7-(i%8)));
    }
  }

}

//inverse側

//逆符号化

//一旦全部 01 に戻す
void inverse_binary_data(unsigned char raw_data, int *inversed_encoded_binary_array, long *inverse_binary_number){
  int i,binary_code[8] = {0};

  for(i=0; i<8; i++){
    if(raw_data%2 == 1){
      binary_code[i] = 1;
    }
    else{
      binary_code[i] = 0;
    }
    raw_data = raw_data/2;
  }
  for(i=0; i<8; i++){
    inversed_encoded_binary_array[*inverse_binary_number] = binary_code[7-i];
    *inverse_binary_number += 1;
  }
}

int serch_dc_table(int *dc_group_binary, int binary_count){
  int i,n;
  bool found_flag = true;
  for(n=0; n<12; n++){
    for(i=0; i<binary_count; i++){
      if(!((int)dc_code_table[n][i] == dc_group_binary[i])){
        found_flag = false;
      }
    }
    if(found_flag){
      return binary_count;
    }
  }

  return -1;
}

int dc_group_judgment(int start_point, int *inversed_encoded_binary_array){
  int i = start_point, dc_group_binary[9]={-1}, dc_group = -1;

  while(dc_group == -1){
    dc_group_binary[i] = inversed_encoded_binary_array[i];
    dc_group = serch_dc_table(dc_group_binary, (i - start_point + 1));

    if(dc_group != -1){
      return dc_group;
    }
    i++;
  }
  return dc_group;
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
