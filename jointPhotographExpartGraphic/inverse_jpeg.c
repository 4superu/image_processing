#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "inverse_function_jpeg.c"
#include "inverse_jpeg_variable.h"

int main(){

  int i,n,j,k,u,v;

  jpeg_image = malloc(sizeof(unsigned char*)*IMAGE_SIZE);
  inverse_quantized_image = malloc(sizeof(unsigned char*)*IMAGE_SIZE);
  inverse_encoded_image = malloc(sizeof(int)*IMAGE_SIZE);
  inverse_encoded_image_array = malloc(sizeof(int*)*32*32);

  for(i=0; i<IMAGE_SIZE; i++){
    jpeg_image[i] = malloc(sizeof(unsigned char)*IMAGE_SIZE);
    inverse_quantized_image[i] = malloc(sizeof(double)*IMAGE_SIZE);
    inverse_encoded_image[i] = malloc(sizeof(int)*IMAGE_SIZE);
  }
  for(i=0; i<32*32; i++){
    inverse_encoded_image_array[i] = malloc(sizeof(double)*64);
  }

  fp = fopen("encoded_data.txt", "rb");
  fread(&encoded_data, sizeof(int),"サイズどうしよう", fp);
  fclose(fp);

  //逆符号化
  dc_flag = true;
  binary_number = 0;
  while(encoded_data[binary_number] != EOF){
    if(dc_flag){
      while(encoded_data[binary_number] == 0){

      }
    }
    encoded_data[binary_number]
  }



  //スキャン??

  //逆量子化
  inverse_quantization(inverse_encoded_image, inverse_quantized_image);

  //逆DCT
  inverseDiscreteCosineTransform(jpeg_image, inverse_quantized_image);

  fp = fopen("lenna_inversed.raw", "wb");
  for(i=0; i<IMAGE_SIZE; i++){
    for (n=0; n<IMAGE_SIZE; n++){
      fwrite(&jpeg_image[i][n], sizeof(unsigned char),1,fp);
    }
  }
  fclose(fp);

}
