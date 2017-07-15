void LabelingFourNearestNeighbor(unsigned char **image_arry, int **labeling_arry, int *fix_labeling_table, int *max_ladeling_number, int image_size_bertical, int image_size_horizontal){
  int i,j;
  *max_ladeling_number = 1;

  for(i=1; i<image_size_bertical-1; i++){
    for(j=1; j<image_size_horizontal-1; j++){
      if(image_arry[i-1][j-1] != 0){

        if(labeling_arry[i][j-1] == 0 && labeling_arry[i-1][j] == 0){
          labeling_arry[i][j] = *max_ladeling_number;
          *max_ladeling_number += 1;
        }
        else if(labeling_arry[i][j-1] == 0 && labeling_arry[i-1][j] != 0){
          labeling_arry[i][j] = labeling_arry[i-1][j];
        }
        else if(labeling_arry[i][j-1] != 0 && labeling_arry[i-1][j] == 0){
          labeling_arry[i][j] = labeling_arry[i][j-1];
        }
        else{
          labeling_arry[i][j] = labeling_arry[i][j-1];
          if(fix_labeling_table[labeling_arry[i][j-1]] != 0){
            fix_labeling_table[labeling_arry[i-1][j]] = fix_labeling_table[labeling_arry[i][j-1]];
          }
          else{
            fix_labeling_table[labeling_arry[i-1][j]] = labeling_arry[i][j-1];
          }
        }
      }
    }
  }
}

void FixLabelingEquivalance(int **labeling_arry, int *fix_labeling_table, int bertical_size, int horizontal_size, int table_size){
  int i,j,n;

  for(n=0; n<table_size; n++){
    for(i=1; i<bertical_size-1; i++){
      for(j=1; j<horizontal_size-1; j++){

        if(fix_labeling_table[n] == labeling_arry[i][j]  && fix_labeling_table[n] != 0){
          labeling_arry[i][j] = n;
        }

      }
    }
  }

}

void FillinLabelingArryBlank(int **labeling_arry, int *fix_labeling_table, int labeling_arry_size_bertical, int labeling_arry_size_horizontal, int fix_labeling_table_size, int *max_ladeling_number){
  int i,j,n;

  for(n=0; n<fix_labeling_table_size; n++){
    if(fix_labeling_table[n] != 0){
      *max_ladeling_number -= 1;
      for(i=1; i<labeling_arry_size_bertical-1; i++){
        for(j=1; j<labeling_arry_size_horizontal-1; j++){
          if(labeling_arry[i][j] > n){
            labeling_arry[i][j]--;
          }
        }
      }
    }
  }
}

void InitializationArryTwoDimennsion(int **arry, int bertical_size, int horizontal_size){
  int i,j;

  for(i=0;i<bertical_size;i++){
    for(j=0;j<horizontal_size;j++){
      arry[i][j] = 0;
    }
  }

}

void InitializationArryOneDimennsion(int *arry, int arry_size){
  int i,j;

  for(i=0;i<arry_size;i++){
    arry[i] = 0;
  }

}

int CheckMaxLabelingNumber(int **labeling_arry, int bertical_size, int horizontal_size){
  int i,j,max=0;

  for(i=0; i<bertical_size; i++){
    for(j=0; j<horizontal_size; j++){
      if(labeling_arry[i][j] > max)
      max = labeling_arry[i][j];
    }
  }
  printf("ラベリング最大値：%d\n",max);
  return max;
}
