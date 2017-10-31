#define BER_SIZE 400
#define HOR_SIZE 600

//画像の濃度ヒストグラムを作成
void StoreImageHistgram(unsigned char **image_arry, int *image_histgram){
  int i, j;

  for(i=0;i<BER_SIZE;i++){
    for(j=0;j<HOR_SIZE;j++){
      image_histgram[image_arry[i][j]]++;
    }
  }
}

//指定された閾値に基づいて、二値化された画像を生成
void GenerateBinaryImage(unsigned char **image_arry, unsigned char **binary_image, int threshould_value){
  int i,j;

  for(i=0;i<BER_SIZE;i++){
    for(j=0;j<HOR_SIZE;j++){
      if(image_arry[i][j] >= threshould_value){
        binary_image[i][j] = 255;
      }
      else{
        binary_image[i][j] = 0;
      }
    }
  }
}
