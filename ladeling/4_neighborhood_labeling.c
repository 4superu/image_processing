#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "memory.h"

#define IMAGE_SIZE 256

int main(){
  int i,j;
  int image_size_bertical=256,image_size_horizontal=256;
  unsigned char **image_arry;
  FILE *fp;

  image_malloc_two_dimension(image_arry, image_size_bertical, image_size_horizontal);
  fp = fopen("../image/lennna_between_class.256", "rb");
  fread(image_arry, sizeof(unsigned char), image_size_bertical*image_size_horizontal, fp);
  fclose(fp);

  for(i=0;i<image_size_bertical;i++){
    for(j=0;j<image_size_horizontal;j++){
      printf("%u",image_arry[i][j]);
    }
    printf("\n");
  }

  free(image_arry);

}
