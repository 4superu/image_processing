#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BER_SIZE 64
#define HOR_SIZE 64
#define DET_SIZE 96

int main(){
  int i,n, angle;
  int **detector;
  unsigned char **image;
  FILE *fp;

  detector = malloc(sizeof(int*)*360);
  for(i=0; i<360; i++){
    detector[i] = malloc(sizeof(int*)*(DET_SIZE));
  }

  image = malloc(sizeof(unsigned char*)*64);
  for(i=0; i<64; i++){
    image[i] = malloc(sizeof(unsigned char)*64);
  }

  fp = fopen("image/lenna_64.raw", "rb");
  for(i=0; i<IMAGE_SIZE; i++){
    for (n=0; n<IMAGE_SIZE; n++){
      fread(&image[i][n], sizeof(unsigned char),1,fp);
    }
  }
  fclose(fp);

  for(angle=0; angle<359; angle++){
    for(i=0; i<BER_SIZE; i++){
      for(n=0; n<HOR_SIZE; n++){
        detector[angle][(int)i*cos(angle)+16] += image[i][n];
      }
    }
  }
}
