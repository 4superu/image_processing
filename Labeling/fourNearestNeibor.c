#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "functionLabeling.h"

#define IMAGE_SIZE 256

int main(){
  int i,j,image_size_bertical,image_size_horizontal;
  int **labeling_arry, *max_ladeling_number, *fix_labeling_table;
  unsigned char **image_arry, **labeling_image_arry;
  FILE *fp;

//画像サイズの指定
  printf("画像サイズ縦：");
  scanf("%d¥n",&image_size_bertical);
  printf("画像サイズ横：");
  scanf("%d¥n",&image_size_horizontal);

//画像の配列確保・格納
  image_arry = (unsigned char **)malloc(sizeof(unsigned char *)*image_size_bertical);
  labeling_image_arry = (unsigned char **)malloc(sizeof(unsigned char *)*image_size_bertical);
  for(int i=0; i<image_size_bertical; i++){
    image_arry[i] = (unsigned char*)malloc(sizeof(unsigned char)*image_size_horizontal);
    labeling_image_arry[i] = (unsigned char*)malloc(sizeof(unsigned char)*image_size_horizontal);
  }
  fp = fopen("../Image/lennna_between_class.256", "rb");
  for(i=0;i<image_size_bertical;i++){
    for (j=0;j<image_size_horizontal;j++){
      fread(&image_arry[i][j], sizeof(unsigned char),1,fp);
    }
  }
  fclose(fp);

//ラベリング配列の確保と初期化 *配列端の処理を容易にするため、上下左右をそれぞれ１行１列づつ拡大*
  labeling_arry = (int **)malloc(sizeof(int *)*image_size_bertical+2);
  for(i=0; i<image_size_bertical+2; i++){
    labeling_arry[i] = (int *)malloc(sizeof(int)*image_size_horizontal+2);
  }
  InitializationArryTwoDimennsion(labeling_arry, image_size_bertical+2, image_size_horizontal+2);

//ラベリング修正用のテーブル
  fix_labeling_table = malloc(sizeof(int)*image_size_bertical*image_size_horizontal/2);
  InitializationArryOneDimennsion(fix_labeling_table, image_size_bertical*image_size_horizontal/2);

//ラベリング番号の最大ラベルを格納
  max_ladeling_number = malloc(sizeof(int)*1);

  //４近傍ラベリング処理
  LabelingFourNearestNeighbor(image_arry, labeling_arry, fix_labeling_table, max_ladeling_number, image_size_bertical+2, image_size_horizontal+2);

  //ラベリングテーブルによる修正処理
  FixLabelingEquivalance(labeling_arry, fix_labeling_table, image_size_bertical+2, image_size_horizontal+2, image_size_bertical*image_size_horizontal/2);

  //修正したことによって出来る不使用ラベル番号を詰める
  FillinLabelingArryBlank(labeling_arry, fix_labeling_table, image_size_bertical+2, image_size_horizontal+2, image_size_bertical*image_size_horizontal/2, max_ladeling_number);

//ラベリング配列の出力と最大ラベンリング番号を出力
  // for(i=1;i<image_size_bertical+1;i++){
  //   for(j=1;j<image_size_horizontal+1;j++){
  //     printf("%d\n",labeling_arry[i][j]);
  //   }
  //   printf("\n");
  // }
  int max_label_number = CheckMaxLabelingNumber(labeling_arry, image_size_bertical+2, image_size_horizontal+2);

  for(i=0; i<image_size_bertical; i++){
    for(j=0; j<image_size_horizontal; j++){
      labeling_image_arry[i][j] = (unsigned char)round((float)labeling_arry[i][j]*(255/(float)max_label_number));
    }
  }

  fp = fopen("../Image/four_nearest_neibor_lenna.256", "wb");
  for(i=0;i<image_size_bertical;i++){
    for (j=0;j<image_size_horizontal;j++){
      fwrite(&labeling_image_arry[i][j], sizeof(int),1,fp);
    }
  }
  fclose(fp);


  free(image_arry);
  free(labeling_arry);
  free(fix_labeling_table);
  free(max_ladeling_number);
}
