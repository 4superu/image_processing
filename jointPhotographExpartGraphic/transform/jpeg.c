#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function_jpeg.c"
#include "jpeg_variable.h"

int main(){

  int i,n,j,k,u,v;

  binary_code = malloc(sizeof(int)*12);
  binary_code_inv = malloc(sizeof(char)*12);
  raw_image = malloc(sizeof(unsigned char*)*IMAGE_SIZE);
  dct_image = malloc(sizeof(double*)*IMAGE_SIZE);
  quantized_image = malloc(sizeof(int*)*IMAGE_SIZE);
  scaned_quantized_image_arry = malloc(sizeof(int*)*1024);
  encode_data_arry = malloc(sizeof(int)*256*256*8);

  for(i=0; i<IMAGE_SIZE; i++){
    raw_image[i] = malloc(sizeof(unsigned char)*IMAGE_SIZE);
    dct_image[i] = malloc(sizeof(double)*IMAGE_SIZE);
    quantized_image[i] = malloc(sizeof(int)*IMAGE_SIZE);
  }
  for(i=0; i<1024; i++){
    scaned_quantized_image_arry[i] = malloc(sizeof(int)*64);
  }

  //lenna読み込み
  fp = fopen("image/lenna.raw", "rb");
  for(i=0; i<IMAGE_SIZE; i++){
    for (n=0; n<IMAGE_SIZE; n++){
      fread(&raw_image[i][n], sizeof(unsigned char),1,fp);
    }
  }
  fclose(fp);

  //DCT
  discreteCosineTransform(raw_image, dct_image);

  //量子化
  quantization(dct_image, quantized_image);

  //ジグザグスキャン
  scan_zigzag(scaned_quantized_image_arry, quantized_image);

  //符号化
  encode_data_number = 0;

  for(block_number=0; block_number<(32*32); block_number++){
    for(element_number=0; element_number<64; element_number++){

      //DC符号化
      if(element_number == 0){
        dc_group_binary(scaned_quantized_image_arry, block_number, &encode_data_number, encode_data_arry);

        binary_conversion(dc_different_value(scaned_quantized_image_arry, block_number), encode_data_arry, &encode_data_number);
      }

      else{
        //AC符号化
        ac_group = select_ac_group(scaned_quantized_image_arry[block_number][element_number
        ]);

        if(ac_group > 0){
          for(i=0; i<ac_length_table[ac_group]; i++){
            encode_data_arry[encode_data_number] = ac_code_table[ac_group][i];
            encode_data_number++;
          }
        }
        else{
          ac_zero_run++;
        }

        if(ac_group != 0){
          add_zero_run_code(encode_data_arry, &encode_data_number, &ac_zero_run);
          binary_conversion(scaned_quantized_image_arry[block_number][element_number], encode_data_arry, &encode_data_number);
        }

        if(element_number == 63){
          add_block_end_binary(encode_data_arry, &encode_data_number);
          ac_zero_run = 0;
        }

      }

    }
  }

  printf("%ld\n",encode_data_number );

  //raw書き込み
  raw_encode_data = malloc(sizeof(unsigned char)*encode_data_number);


  for(i=0; i<encode_data_number; i++){
    if(encode_data_arry[i] == 1){
      raw_encode_data[i/8] += pow(2,(7-(i%8)));
    }
  }

  fp = fopen("image/encode_data.raw", "wb");
  fwrite(raw_encode_data, sizeof(unsigned char), 256*256, fp);
  fclose(fp);


  fclose(fp);

  free(raw_image);
  free(dct_image);
  free(quantized_image);
  free(scaned_quantized_image_arry);
  free(binary_code);
  free(binary_code_inv);
  free(encode_data_arry);
}
