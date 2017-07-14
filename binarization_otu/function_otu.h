
#define IMAGE_SIZE_LENNA 256
//画像の濃度ヒストグラムを作成
void store_image_histgram(unsigned char *image_arry, int *image_histgram){
  int i, j;

  for(i=0;i<IMAGE_SIZE_LENNA;i++){
    for(j=0;j<IMAGE_SIZE_LENNA;j++){
      image_histgram[image_arry[i*IMAGE_SIZE_LENNA+j]]++;
    }
  }
}

//画像全体の濃度値の平均を算出
float calculate_image_pixel_average(unsigned char *image_arry){
  int i,j;
  float image_average=0;
  for(i=0;i<IMAGE_SIZE_LENNA;i++){
    for(j=0;j<IMAGE_SIZE_LENNA;j++){
      image_average += image_arry[i*IMAGE_SIZE_LENNA+j];
    }
  }
  return image_average/(IMAGE_SIZE_LENNA*IMAGE_SIZE_LENNA);
}

//ヒストグラム配列の任意の範囲での平均値を算出
float calculate_histgram_average(int start, int end, int *image_histgram){
  int i;
  float hist_average=0;

  for(i=start; i<end+1; i++){
    hist_average += image_histgram[i];
  }
  return hist_average/(end-start+1);
}

//ヒストグラム配列の任意の範囲での分散を算出
float calculate_histgram_variance(int start, int end, int *image_histgram, float histgram_average){
  int i;
  float variance=0;
  for(i=start;i<end+1;i++){
    variance += pow((image_histgram[i] - histgram_average), 2);
  }
  return variance/(end-start+1);
}

//ヒストグラム配列の任意の範囲の要素数を算出
int calculate_total_arry_element(int start, int end, int *histgram){
  int i,sum=0;
  for(i=start; i<end+1; i++){
    sum += histgram[i];
  }
  return sum;
}

//分散配列から最小値を取る配列番号を計算
int smallest_class_variance_threshould(float *class_variance_arry){
  int i,threshould=0;
  for(i=threshould;i<255;i++){
    if(class_variance_arry[threshould] > class_variance_arry[i]){
      threshould = i;
    }
  }
  return threshould;
}

//分散配列から最大値をとる配列番号を計算
int largest_between_class_variance_threshould(float *between_class_variance_arry){
  int i,threshould=0;
  for(i=threshould;i<255;i++){
    if(between_class_variance_arry[threshould] < between_class_variance_arry[i]){
      threshould = i;
    }
  }
  return threshould;
}

//指定された閾値に基づいて、二値化された画像を生成
void generate_binary_image(unsigned char *image_arry, unsigned char *binary_image, int threshould_value){
  int i,j;

  for(i=0;i<IMAGE_SIZE_LENNA;i++){
    for(j=0;j<IMAGE_SIZE_LENNA;j++){
      if(image_arry[i*IMAGE_SIZE_LENNA+j] >= threshould_value){
        binary_image[i*IMAGE_SIZE_LENNA+j] = 255;
      }
      else{
        binary_image[i*IMAGE_SIZE_LENNA+j] = 0;
      }
    }
  }
}
