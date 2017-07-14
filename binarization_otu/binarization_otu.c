#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "function_otu.h"

#define IMAGE_SIZE_LENNA 256

int main(){

  int i, j, class_threshould, between_class_threshould;
  int total_arry_element_left, total_arry_element_right;
  int *image_histgram;
  float variance_left, variance_right, *class_variance_arry, *between_class_variance_arry;
  float image_pixel_average, hist_average_left, hist_average_right;
  unsigned char *image_arry, *binary_image_arry_in_class, *binary_image_arry_between_class;
  FILE *fp;

// 画像を読み込み、配列に格納
  image_arry = (unsigned char*)malloc(sizeof(unsigned char)*IMAGE_SIZE_LENNA*IMAGE_SIZE_LENNA);
  fp = fopen("lenna.256", "rb");
  fread(image_arry, sizeof(unsigned char), IMAGE_SIZE_LENNA*IMAGE_SIZE_LENNA, fp);
  fclose(fp);

//画像のヒストグラム作成
  image_histgram = (int*)malloc(sizeof(int)*IMAGE_SIZE_LENNA);
  for(i=0;i<256;i++){
    image_histgram[i]=0;
  }
  store_image_histgram(image_arry, image_histgram);

//画像の総画素値の平均値
  image_pixel_average = calculate_image_pixel_average(image_arry);

//クラス分けを行い、それぞれ計算する
  class_variance_arry = (float*)malloc(sizeof(float)*256);
  between_class_variance_arry = (float*)malloc(sizeof(float)*256);
  for(i=0;i<256;i++){
    class_variance_arry[i] = 0;
    between_class_variance_arry[i] = 0;
  }
  for(i=1; i<255; i++){
    //左側クラスの計算
    hist_average_left = calculate_histgram_average(0,i,image_histgram);
    variance_left = calculate_histgram_variance(0,i,image_histgram,hist_average_left);
    total_arry_element_left = calculate_total_arry_element(0,i,image_histgram);

    //右側クラスの計算
    hist_average_right = calculate_histgram_average(i+1, IMAGE_SIZE_LENNA, image_histgram);
    variance_right = calculate_histgram_variance(i+1,IMAGE_SIZE_LENNA,image_histgram,hist_average_right);
    total_arry_element_right = calculate_total_arry_element(i+1,IMAGE_SIZE_LENNA,image_histgram);

    //各閾値での分散値を格納
    class_variance_arry[i] = (total_arry_element_left*variance_left + total_arry_element_right*variance_right)/(total_arry_element_left+total_arry_element_right);
    between_class_variance_arry[i] = total_arry_element_left*total_arry_element_right*pow((hist_average_left - hist_average_right),2)/pow((total_arry_element_left - total_arry_element_right),2);

  }

  class_threshould = smallest_class_variance_threshould(class_variance_arry);
  between_class_threshould = largest_between_class_variance_threshould(between_class_variance_arry);

  for(i=1;i<254;i++){
    printf("%f  %f\n",class_variance_arry[i],between_class_variance_arry[i]);
  }
  printf("\n%d %d\n\n",class_threshould, between_class_threshould );

  binary_image_arry_in_class = (unsigned char*)malloc(sizeof(unsigned char)*IMAGE_SIZE_LENNA*IMAGE_SIZE_LENNA);
  binary_image_arry_between_class = (unsigned char*)malloc(sizeof(unsigned char)*IMAGE_SIZE_LENNA*IMAGE_SIZE_LENNA);


  generate_binary_image(image_arry, binary_image_arry_in_class, class_threshould);
  generate_binary_image(image_arry, binary_image_arry_between_class, between_class_threshould);

  fp = fopen("lennna_between_class.256", "wb");
  fwrite(binary_image_arry_between_class, sizeof(unsigned char), IMAGE_SIZE_LENNA*IMAGE_SIZE_LENNA, fp);
  fclose(fp);

  fp = fopen("lennna_in_class.256", "wb");
  fwrite(binary_image_arry_in_class, sizeof(unsigned char), IMAGE_SIZE_LENNA*IMAGE_SIZE_LENNA, fp);
  fclose(fp);

  free(class_variance_arry);
  free(between_class_variance_arry);
  free(image_arry);
  free(binary_image_arry_in_class);
  free(binary_image_arry_between_class);

}
