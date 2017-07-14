
#define IMAGE_SIZE_LENNA 256

void store_image_histgram(unsigned char *image_arry, int *image_histgram){
  int i, j;

  for(i=0;i<IMAGE_SIZE_LENNA;i++){
    for(j=0;j<IMAGE_SIZE_LENNA;j++){
      image_histgram[image_arry[i*IMAGE_SIZE_LENNA+j]]++;
    }
  }
}

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

float calculate_histgram_average(int start, int end, int *image_histgram){
  int i;
  float hist_average=0;

  for(i=start; i<end+1; i++){
    hist_average += image_histgram[i];
  }
  return hist_average/(end-start+1);
}

float calculate_histgram_variance(int start, int end, int *image_histgram, float histgram_average){
  int i;
  float variance=0;
  for(i=start;i<end+1;i++){
    variance += pow((image_histgram[i] - histgram_average), 2);
  }
  return variance/(end-start+1);
}

int calculate_total_arry_element(int start, int end, int *histgram){
  int i,sum=0;
  for(i=start; i<end+1; i++){
    sum += histgram[i];
  }
  return sum;
}

int smallest_class_variance_threshould(float *class_variance_arry){
  int i,threshould=1;
  for(i=threshould;i<255;i++){
    if(class_variance_arry[threshould] > class_variance_arry[i]){
      threshould = i;
    }
  }
  return threshould;
}

int largest_between_class_variance_threshould(float *between_class_variance_arry){
  int i,threshould=1;
  for(i=threshould;i<255;i++){
    if(between_class_variance_arry[threshould] < between_class_variance_arry[i]){
      threshould = i;
    }
  }
  return threshould;
}

void generate_binary_image(unsigned char *image_arry, unsigned char *binary_image, int threshould_value){
  int i,j;

  for(i=0;i<IMAGE_SIZE_LENNA;i++){
    for(j=0;j<IMAGE_SIZE_LENNA;j++){
      if(image_arry[i*IMAGE_SIZE_LENNA+j] > threshould_value){
        binary_image[i*IMAGE_SIZE_LENNA+j] = 255;
      }
      else{
        binary_image[i*IMAGE_SIZE_LENNA+j] = 0;
      }
    }
  }
}
