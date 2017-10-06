#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function_jpeg.c"
#include "jpeg_variable.h"

int main(){

  int i,n,j,k,u,v;

  binary_code = malloc(sizeof(char)*12);
  binary_code_inv = malloc(sizeof(char)*12);
  raw_image = malloc(sizeof(unsigned char*)*IMAGE_SIZE);
  dct_image = malloc(sizeof(double*)*IMAGE_SIZE);
  quantized_image = malloc(sizeof(int*)*IMAGE_SIZE);

  scaned_quantized_image_arry = malloc(sizeof(int*)*1024);

  for(i=0; i<IMAGE_SIZE; i++){
    raw_image[i] = malloc(sizeof(unsigned char)*IMAGE_SIZE);
    dct_image[i] = malloc(sizeof(double)*IMAGE_SIZE);
    quantized_image[i] = malloc(sizeof(int)*IMAGE_SIZE);
  }
  for(i=0; i<1024; i++){
    scaned_quantized_image_arry[i] = malloc(sizeof(int)*64);
  }

  //lenna読み込み
  fp = fopen("lenna.raw", "rb");
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
  fp = fopen("encoded_data.txt", "a");

  for(block_number=0; block_number<32*32; block_number++){
    for(element_number=0; element_number<64; element_number++){

      //DC符号化
      if(element_number == 0){
        dc_group = select_dc_group(scaned_quantized_image_arry, block_number);//dcのssss求める

        for(i=0; i<dc_length_table[dc_group]; i++){
          fprintf(fp, "%d", dc_code_table[dc_group][i]);
        }

        diff_value = dc_different_value(scaned_quantized_image_arry, block_number);

        if(diff_value > 0){
          snprintf(binary_code, 12, "%ld", binary_conversion(diff_value));
          fprintf(fp, "%s", binary_code);
        }
        else if(diff_value < 0){
          binary_conversion_negative(diff_value, binary_code, &bit_number);
          while(bit_number > 0){
            fprintf(fp, "%d", binary_code[bit_number-1]);
            bit_number--;
          }
        }

      }

      //AC符号化
      int ac_value=0;
      ac_zero_run = 0;
      else{
        ac_group = select_ac_group(scaned_quantized_image_arry[block_number][element_number]);
        if(ac_group != 0){
          for(i=0; i<ac_length_table[*****]; i++){
            fprintf(fp, "%d", ac_code_table[******][i] );
          }
          ac_zero_run = 0;
        }
        else{
          ac_zero_run++;
        }
        ac_value = scaned_quantized_image_arry[block_number][element_number];
        if(ac_value > 0){
          snprintf(binary_code, 12, "%ld", binary_conversion(ac_value);
          fprintf(fp, "%s", binary_code);
        }
        else if(ac_value < 0){
          binary_conversion_negative(ac_value, binary_code, &bit_number);
          while(bit_number > 0){
            fprintf(fp, "%d", binary_code[bit_number-1]);
            bit_number--;
          }
        }

      }

    }
  }
  fclose(fp);



  free(raw_image);
  free(dct_image);
  free(quantized_image);
  free(scaned_quantized_image_arry);
  free(binary_code);
  free(binary_code_inv);

}
