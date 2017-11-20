#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function/function_jpeg.c"
#include "variable/jpeg_variable.h"
#include "variable/inverse_jpeg_variable.h"

int main(){

  int i,n,j,k,u,v;
  int binary_length[32*32] = {0};

//transform変数
  raw_image = malloc(sizeof(unsigned char*)*IMAGE_SIZE);
  dct_image = malloc(sizeof(double*)*IMAGE_SIZE);
  quantized_image = malloc(sizeof(int*)*IMAGE_SIZE);
  scaned_quantized_image_arry = malloc(sizeof(int*)*1024);
  encode_data_arry = malloc(sizeof(int)*256*256*8);
//inverse変数
  inv_raw_image = malloc(sizeof(double*)*IMAGE_SIZE);
  quantized_image_inverse = malloc(sizeof(double*)*IMAGE_SIZE);
  scaned_freaquency_image = malloc(sizeof(double*)*IMAGE_SIZE);
  inversed_frequency_component_arry = malloc(sizeof(int*)*1024);

  for(i=0; i<IMAGE_SIZE; i++){
    raw_image[i] = malloc(sizeof(unsigned char)*IMAGE_SIZE);
    dct_image[i] = malloc(sizeof(double)*IMAGE_SIZE);
    quantized_image[i] = malloc(sizeof(int)*IMAGE_SIZE);

    inv_raw_image[i] = malloc(sizeof(double)*IMAGE_SIZE);
    quantized_image_inverse[i] = malloc(sizeof(double)*IMAGE_SIZE);
    scaned_freaquency_image[i] = malloc(sizeof(double)*IMAGE_SIZE);
  }

  for(i=0; i<1024; i++){
    scaned_quantized_image_arry[i] = malloc(sizeof(int)*64);
    inversed_frequency_component_arry[i] = malloc(sizeof(int)*64);
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
  ac_zero_run = 0;

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
        ///debug_point
        if(ac_group != 0){
          add_zero_run_code(encode_data_arry, &encode_data_number, &ac_zero_run);
          ///debug_point
          ac_group_binary(encode_data_arry, &encode_data_number, ac_group, &ac_zero_run);
          ///debug_point
          binary_conversion(scaned_quantized_image_arry[block_number][element_number], encode_data_arry, &encode_data_number);
          ///debug_point
        }
        else{
          ac_zero_run++;
        }
        if(element_number == 63){
          add_block_end_binary(encode_data_arry, &encode_data_number);
          ac_zero_run = 0;
          // binary_length[block_number] = encode_data_number;
        }
      }
    }
  }
  // printf("%ld\n",encode_data_number );
  // for(i=0; i<32*32; i++){
  //   for(n=0; n<64; n++){
  //     printf("%d ",scaned_quantized_image_arry[i][n] );
  //   }
  //   printf("\n\n");
  // }

  inversed_encoded_binary_array = malloc(sizeof(int)*(encode_data_number+8));

  //raw書き込み
  raw_encode_data = malloc(sizeof(unsigned char)*encode_data_number);
  compression_to_raw(raw_encode_data, &encode_data_number, encode_data_arry);

  fp = fopen("image/encode_data.raw", "wb");
  fwrite(raw_encode_data, sizeof(unsigned char), 256*256, fp);
  fclose(fp);

  free(raw_image);
  free(dct_image);
  free(quantized_image);
  // free(scaned_quantized_image_arry);
  //transform終了

  //inverse側
  fp = fopen("image/encode_data.raw","rb");
  fread(raw_encode_data, sizeof(unsigned char), encode_data_number, fp);
  fclose(fp);

  inverse_binary_number = 0;
  //一旦全部 01 に戻す
  for(i=0; i<(encode_data_number/8+1); i++){
    inverse_binary_data(raw_encode_data[i], inversed_encoded_binary_array, &inverse_binary_number);
  }

  // 01からブロックごとの数値に戻す
  inverse_binary_progress = 0;
  pre_dc_value = 0;
  block_number = 0, element_number = 0;

  while(block_number < 32*32){
    //DC逆符号化
    if(dc_flag){
      //dc_group探す
      dc_group = dc_group_judgment(inverse_binary_progress, inversed_encoded_binary_array);
      //dc_groupの符号分進める
      inverse_binary_progress += (int)dc_length_table[dc_group];
      //dc値決める☆☆
      dc_value = integer_conversion(inverse_binary_progress, inversed_encoded_binary_array, dc_group) + pre_dc_value;

      //配列に入れて、前のdc値更新
      inversed_frequency_component_arry[block_number][0] = dc_value;
      pre_dc_value = dc_value;

      //dc_値分のビット進める
      if(!(dc_group == 0)){
        inverse_binary_progress += dc_group;
      }
      //acに移行
      dc_flag = false;
      element_number = 1;
    }
    else{
      // 終わりビット判定（おそらくOK）
      end_block_flag = true;
      for(i=0; i<4; i++){
        if(!(inversed_encoded_binary_array[inverse_binary_progress+i] == (i+1)%2)){
          end_block_flag = false;
        }
      }
      if(end_block_flag){
        for(i=element_number; i<64; i++){
          inversed_frequency_component_arry[block_number][i] = 0;
        }
        element_number = 0;
        inverse_binary_progress += 4;
      }

      //特殊ビット判定（ ダメ ）原因は他？
      if(element_number != 0){
        special_bit_flag = true;
        while(special_bit_flag){
          for(i=0; i<(int)ac_length_table[165]; i++){
            if(inversed_encoded_binary_array[inverse_binary_progress+i] != (int)ac_code_table[165][i]){
              special_bit_flag = false;
            }
          }
          if(special_bit_flag){
            inverse_binary_progress += 11;
            for(i=0; i<15; i++){
              inversed_frequency_component_arry[block_number][element_number] = 0;
              element_number++;
            }
          }
        }
        // ac_group選択
        ac_vlc = ac_group_judgment(inverse_binary_progress, inversed_encoded_binary_array);

        ac_zero_run = ac_vlc/11;
        ac_group = ac_vlc%11;
        inverse_binary_progress += (int)ac_length_table[ac_vlc];

        for(j=0; j<ac_zero_run; j++){
          inversed_frequency_component_arry[block_number][element_number] = 0;
          element_number++;
        }
        //ac値判定
        ac_value = integer_conversion(inverse_binary_progress, inversed_encoded_binary_array, ac_group);
        // printf("%d: %d\n",ac_group, ac_value );
        inverse_binary_progress += ac_group;
        inversed_frequency_component_arry[block_number][element_number] = ac_value;
        element_number++;
      }
      else{
        block_number++;
        element_number = 0;
        dc_flag = true;
      }

    }
  }

  inverse_scan_zigzag(inversed_frequency_component_arry, scaned_freaquency_image);

  inverse_quantization(scaned_freaquency_image, quantized_image_inverse);

  inverseDiscreteCosineTransform(inv_raw_image, quantized_image_inverse);

  fp = fopen("image/inv_lenna.raw", "wb");
  for(i=0; i<IMAGE_SIZE; i++){
    for (n=0; n<IMAGE_SIZE; n++){
      fwrite(&inv_raw_image[i][n], sizeof(double),1,fp);
    }
  }
  fclose(fp);

  free(inv_raw_image);
  free(quantized_image_inverse);
  free(scaned_freaquency_image);
  free(inversed_encoded_binary_array);
  free(inversed_frequency_component_arry);
}
