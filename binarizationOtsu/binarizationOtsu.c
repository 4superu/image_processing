#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functionOtsu.c"

#define BER_SIZE 600
#define HOR_SIZE 400

int main(){

  int i, j;
  int threshould_value, total_element_left, total_element_right;
  long total_value_left, total_value_right;

  int *image_histgram;//画像を元に作ったヒストグラム

  double image_pixel_average, left_class_average, rigth_class_average;
  double degree_separation = 0.0, max_degree_separation = 0.0;
  unsigned char **image, **binary_image;//二種類の閾値に基づいて作成する画像配列
  FILE *fp;

// 画像を読み込み、配列に格納
  image = malloc(sizeof(unsigned char*)*BER_SIZE);
  for(i=0; i<BER_SIZE; i++){
    image[i] = malloc(sizeof(unsigned char)*HOR_SIZE);
  }

  fp = fopen("Image/lenna.raw", "rb");
  for(i=0; i<BER_SIZE; i++){
    for(j=0; j<HOR_SIZE; j++){
      fread(&image[i][j], sizeof(unsigned char), 1, fp);
    }
  }
  fclose(fp);

//画像のヒストグラム作成
  image_histgram = malloc(sizeof(int)*256);
  for(i=0;i<256;i++){
    image_histgram[i]=0;
  }
  StoreImageHistgram(image, image_histgram);

// クラス分けを行い、それぞれ計算する

  for(i=1; i<256; i++){

    total_element_left = 0;
    total_element_right = 0;
    total_value_left = 0;
    total_value_right = 0;

    for(j=0; j<=i; j++){
      total_element_left += image_histgram[j];
      total_value_left += image_histgram[j] * j;
    }
    for(j=i+1; j<256; j++){
      total_element_right += image_histgram[j];
      total_value_right += image_histgram[j] * j;
    }

    left_class_average = (double)total_value_left/(double)total_element_left;
    rigth_class_average = (double)total_value_right/(double)total_element_right;

    degree_separation = (double)total_element_right * (double)total_element_left * pow((double)(left_class_average - rigth_class_average),2);

    if(degree_separation > max_degree_separation){
      max_degree_separation = degree_separation;
      threshould_value = i;
    }
  }

  printf("%d\n",threshould_value);

  binary_image = malloc(sizeof(unsigned char*)*BER_SIZE);
  for(i=0; i<BER_SIZE; i++){
    binary_image[i] = malloc(sizeof(unsigned char)*HOR_SIZE);
  }

//閾値によって二値化画像配列を生成
  GenerateBinaryImage(image, binary_image, threshould_value);

// 画像書き込み

  fp = fopen("Image/lenna_otsu.raw", "wb");
  for(i=0; i<BER_SIZE; i++){
    for(j=0; j<HOR_SIZE; j++){
      fwrite(&binary_image[i][j], sizeof(unsigned char), 1, fp);
    }
  }
  fclose(fp);

//メモリ解放
  free(image);
  free(binary_image);
  free(image_histgram);
}
