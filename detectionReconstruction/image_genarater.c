#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
  int i,n;
  float x,y;
  float *circle;
  FILE *fp;

  circle = (float*)malloc(sizeof(float)*10000);

  for(i=0; i<100; i++){
    for(n=0; n<100; n++){
      x = (i+0.5) - 50.0;
      y = 50.0 - (n+0.5);

      if((x*x+y*y)<900.0){
        circle[i*100+n] = 255.0;
      }
      else{
        circle[i*100+n] = 0.0;
      }
    }
  }

  fp = fopen("./image/circle_100-100.raw","wb");
  fwrite(circle, sizeof(float), 10000, fp);
  fclose(fp);
}
