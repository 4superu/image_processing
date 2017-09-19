#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functionOtsu.h"

#define IMAGE_SIZE_LENNA 256

int main(){

  int i, j, class_threshould, between_class_threshould, compare_variance_threshould;//ループ変数と２つの閾値
  int total_arry_element_left, total_arry_element_right;//それぞれのクラスでの画素数
  int *image_histgram;//画像を元に作ったヒストグラム
  float variance_left, variance_right, *class_variance_arry, *between_class_variance_arry, *compare_variance_arry;//それぞれのクラスでの分散、２種類の分散を格納する配列
  float image_pixel_average, hist_average_left, hist_average_right;//全画素値平均,各クラスヒストグラムの平均値
  unsigned char *image_arry, *binary_image_arry_in_class, *binary_image_arry_between_class, *binary_image_compare_variance;//二種類の閾値に基づいて作成する画像配列
  FILE *fp;

// 画像を読み込み、配列に格納
  image_arry = (unsigned char*)malloc(sizeof(unsigned char)*IMAGE_SIZE_LENNA*IMAGE_SIZE_LENNA);
  fp = fopen("../Image/lenna.256", "rb");
  fread(image_arry, sizeof(unsigned char), IMAGE_SIZE_LENNA*IMAGE_SIZE_LENNA, fp);
  fclose(fp);

//画像のヒストグラム作成
  image_histgram = (int*)malloc(sizeof(int)*IMAGE_SIZE_LENNA);
  for(i=0;i<256;i++){
    image_histgram[i]=0;
  }
  StoreImageHistgram(image_arry, image_histgram);

//CSVファイルにヒストグラム配列を書き出す
  fp = fopen("image_histgram.csv","w");
  for(i=0; i<256; i++){
    fprintf(fp ,"%d\n",image_histgram[i]);
  }
  fclose(fp);

//画像の総画素値の平均値
  image_pixel_average = CalculateImagePixelAverage(image_arry);

//クラス分けを行い、それぞれ計算する
  class_variance_arry = (float*)malloc(sizeof(float)*255);
  between_class_variance_arry = (float*)malloc(sizeof(float)*255);
  compare_variance_arry = (float*)malloc(sizeof(float)*255);

  for(i=0;i<255;i++){
    class_variance_arry[i] = 0;
    between_class_variance_arry[i] = 0;
    compare_variance_arry[i] = 0;

  }
  for(i=0; i<255; i++){
    //左側クラスの計算
    hist_average_left = CalculateHistgramAverage(0,i,image_histgram);
    variance_left = CalculateHistgramVariance(0,i,image_histgram,hist_average_left);
    total_arry_element_left = CalculateTotalArryElement(0,i,image_histgram);

    //右側クラスの計算
    hist_average_right = CalculateHistgramAverage(i+1, IMAGE_SIZE_LENNA-1, image_histgram);
    variance_right = CalculateHistgramVariance(i+1,IMAGE_SIZE_LENNA-1,image_histgram,hist_average_right);
    total_arry_element_right = CalculateTotalArryElement(i+1,IMAGE_SIZE_LENNA-1,image_histgram);

    //各閾値での分散値を格納
    class_variance_arry[i] = (total_arry_element_left*variance_left + total_arry_element_right*variance_right)/(total_arry_element_left+total_arry_element_right);
    between_class_variance_arry[i] = total_arry_element_left*total_arry_element_right*pow((hist_average_left - hist_average_right),2)/pow((total_arry_element_left + total_arry_element_right),2);
    compare_variance_arry[i] = between_class_variance_arry[i]/class_variance_arry[i];
  }

//クラス内分散とクラス間分散の最適値を取る閾値をそれぞれ出力
  class_threshould = SmallestClassVarianceThreshould(class_variance_arry);
  between_class_threshould = LargestBetweenClassVarianceThreshould(between_class_variance_arry);
  compare_variance_threshould = LargestBetweenClassVarianceThreshould(compare_variance_arry);

//それぞれの閾値での分散値と最適な閾値を出力
  printf("θ値 クラス内分散和 クラス間分散　分散比　\n");
  for(i=0;i<255;i++){
    printf("%3d  %5.6f  %5.6f\n",i,class_variance_arry[i],between_class_variance_arry[i]);
  }
  printf("\nクラス内分散和の最小値を取る閾値：%d　クラス間分散の最大値を取る閾値：%d それぞれの比による閾値：%d \n\n",class_threshould, between_class_threshould, compare_variance_threshould);

//それぞれの閾値に基づき生成する画像
  binary_image_arry_in_class = (unsigned char*)malloc(sizeof(unsigned char)*IMAGE_SIZE_LENNA*IMAGE_SIZE_LENNA);
  binary_image_arry_between_class = (unsigned char*)malloc(sizeof(unsigned char)*IMAGE_SIZE_LENNA*IMAGE_SIZE_LENNA);
  binary_image_compare_variance = (unsigned char*)malloc(sizeof(unsigned char)*IMAGE_SIZE_LENNA*IMAGE_SIZE_LENNA);

//閾値によって二値化画像配列を生成
  GenerateBinaryImage(image_arry, binary_image_arry_in_class, class_threshould);
  GenerateBinaryImage(image_arry, binary_image_arry_between_class, between_class_threshould);
  GenerateBinaryImage(image_arry, binary_image_compare_variance, compare_variance_threshould);

// 画像書き込み
  fp = fopen("../Image/lennna_between_class.256", "wb");
  fwrite(binary_image_arry_between_class, sizeof(unsigned char), IMAGE_SIZE_LENNA*IMAGE_SIZE_LENNA, fp);
  fclose(fp);

  fp = fopen("../Image/lennna_in_class.256", "wb");
  fwrite(binary_image_arry_in_class, sizeof(unsigned char), IMAGE_SIZE_LENNA*IMAGE_SIZE_LENNA, fp);
  fclose(fp);

  fp = fopen("../Image/lennna_compare_class.256", "wb");
  fwrite(binary_image_compare_variance, sizeof(unsigned char), IMAGE_SIZE_LENNA*IMAGE_SIZE_LENNA, fp);
  fclose(fp);

//メモリ解放
  free(class_variance_arry);
  free(between_class_variance_arry);
  free(image_arry);
  free(binary_image_arry_in_class);
  free(binary_image_arry_between_class);

}
