#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define IMAGE_SIZE 100
#define DET_SIZE 150

int main(){
  int i,n, angle, det, detector_coord;
  float s, t, x , y, pr;
  float *rec_image;
  float *detector;
  float *image;
  FILE *fp;

  detector = (float*)calloc((DET_SIZE*360),sizeof(float));
  rec_image = (float*)calloc((IMAGE_SIZE*IMAGE_SIZE),sizeof(int));
  image = (float*)malloc(sizeof(float)*(IMAGE_SIZE*IMAGE_SIZE));

  fp = fopen("./image/circle_100-100.raw", "rb");
  fread(image, sizeof(float),IMAGE_SIZE*IMAGE_SIZE,fp);
  fclose(fp);

  clock_t start,end;
  start = clock();

  // for(int j=0; j<100; j++){
    for(angle=0; angle<360; angle++){
      for(i=0; i<IMAGE_SIZE; i++){
        x = (i+0.5) - (IMAGE_SIZE/2.0);
        for(n=0; n<IMAGE_SIZE; n++){
          y = (IMAGE_SIZE/2.0) - (n+0.5);

          s = x*cos(angle*M_PI/180.0) - y*sin(angle*M_PI/180.0);
          // t = x*sin(angle*M_PI/180) + y*cos(angle*M_PI/180);
          pr = s - floorf(s);

          detector_coord = (int)(floorf(s)+(DET_SIZE/2.0));

          // detector[angle*DET_SIZE+detector_coord] += (int)image[i*IMAGE_SIZE+n];
          detector[angle*DET_SIZE+detector_coord] += image[n*IMAGE_SIZE+i]*(1.0-pr);
          detector[angle*DET_SIZE+detector_coord+1] += image[n*IMAGE_SIZE+i]*pr;

        }
      }
    }


    for(angle=0; angle<360; angle++){
      for(i=0; i<IMAGE_SIZE; i++){
        x = (i+0.5) - (IMAGE_SIZE/2.0);
        for(n=0; n<IMAGE_SIZE; n++){
          y = (IMAGE_SIZE/2.0) - (n+0.5);
          s = x*cos(angle*M_PI/180.0) - y*sin(angle*M_PI/180.0);
          // t = x*sin(angle*M_PI/180) + y*cos(angle*M_PI/180);

          detector_coord = (int)(floorf(s)+(DET_SIZE/2.0));
          pr = s - floor(s);

          // rec_image[n*IMAGE_SIZE+i] += detector[angle*DET_SIZE+detector_coord];
          rec_image[n*IMAGE_SIZE+i] += detector[angle*DET_SIZE+detector_coord]*(1-pr);
          rec_image[n*IMAGE_SIZE+i] += detector[angle*DET_SIZE+detector_coord+1]*pr;

        }
      }
    }
  // }

  end = clock();
  printf("%.2f秒かかりました\n",(double)(end-start)/CLOCKS_PER_SEC);


  // fp = fopen("image/circle_detection_150-360.raw", "wb");
  fp = fopen("./image/inter_circle_detection_150-360.raw", "wb");
  fwrite(detector, sizeof(float),DET_SIZE*360,fp);
  fclose(fp);

  // fp = fopen("image/rec_circle_100-100.raw", "wb");
  fp = fopen("./image/inter_rec_circle_100-100.raw", "wb");
  fwrite(rec_image, sizeof(float),IMAGE_SIZE*IMAGE_SIZE,fp);
  fclose(fp);

  free(image);
  free(rec_image);
  free(detector);


}
