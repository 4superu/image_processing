#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cuda_runtime.h>

#define IMAGE_SIZE 100
#define DET_SIZE 150

__global__ void multiDetection(float *cuda_detector, float *cuda_image){
  int i,j,angle,detector_coord;
  float x,y,s,pr;

  angle = blockIdx.x;
  i= blockIdx.y * blockDim.y + threadIdx.y;
  j= blockIdx.z * blockDim.z + threadIdx.z;

  x = (i+0.5) - (IMAGE_SIZE/2.0);
  y = (IMAGE_SIZE/2.0) - (j+0.5);

  s = x*cos(angle*M_PI/180.0) - y*sin(angle*M_PI/180.0);
  pr = s - floor(s);

  detector_coord = (int)(floor(s)+(DET_SIZE/2.0));

  atomicAdd(&cuda_detector[angle*DET_SIZE+detector_coord], cuda_image[j*IMAGE_SIZE+i]*(1.0-pr));
  atomicAdd(&cuda_detector[angle*DET_SIZE+detector_coord+1], cuda_image[j*IMAGE_SIZE+i]*pr);
}

__global__ void multiReconstruction(float *cuda_detector, float *cuda_rec_image){
  int i,j,angle,detector_coord;
  float x,y,s,pr;

  angle = blockIdx.x;
  i= blockIdx.y * blockDim.y + threadIdx.y;
  j= blockIdx.z * blockDim.z + threadIdx.z;

  x = (i+0.5) - (IMAGE_SIZE/2.0);
  y = (IMAGE_SIZE/2.0) - (j+0.5);

  s = x*cos(angle*M_PI/180.0) - y*sin(angle*M_PI/180.0);
  pr = s - floor(s);

  detector_coord = (int)(floor(s)+(DET_SIZE/2.0));

  atomicAdd(&cuda_rec_image[j*IMAGE_SIZE+i], cuda_detector[angle*DET_SIZE+detector_coord]*(1-pr));
  atomicAdd(&cuda_rec_image[j*IMAGE_SIZE+i], cuda_detector[angle*DET_SIZE+detector_coord+1]*pr);
}

int main(){
  float *image, *cuda_image;
  float *rec_image, *cuda_rec_image;
  float *detector, *cuda_detector;
  float time_ms = 0.0f;
  cudaEvent_t start, stop;
  FILE *fp;

  image = (float*)malloc(sizeof(float)*(IMAGE_SIZE*IMAGE_SIZE));
  rec_image = (float*)calloc((IMAGE_SIZE*IMAGE_SIZE),sizeof(float));
  detector = (float*)calloc((DET_SIZE*360),sizeof(float));
  fp = fopen("./image/circle_100-100.raw", "rb");

  fread(image, sizeof(float), IMAGE_SIZE*IMAGE_SIZE, fp);
  fclose(fp);

  cudaEventCreate(&start);
  cudaEventCreate(&stop);

  cudaMalloc((void**)&cuda_detector, sizeof(float)*DET_SIZE*360);
  cudaMalloc((void**)&cuda_image, sizeof(float)*IMAGE_SIZE*IMAGE_SIZE);
  cudaMalloc((void**)&cuda_rec_image, sizeof(float)*IMAGE_SIZE*IMAGE_SIZE);

  cudaMemcpy(cuda_image, image, sizeof(float)*IMAGE_SIZE*IMAGE_SIZE, cudaMemcpyHostToDevice);
  cudaMemcpy(cuda_detector, detector, sizeof(float)*DET_SIZE*360, cudaMemcpyHostToDevice);
  cudaMemcpy(cuda_rec_image, rec_image, sizeof(float)*IMAGE_SIZE*IMAGE_SIZE, cudaMemcpyHostToDevice);

  cudaEventRecord(start, 0);

  dim3 blocks(360,10,10);
  dim3 threads(1,10,10);

  multiDetection<<< blocks, threads >>>(cuda_detector, cuda_image);
  cudaThreadSynchronize();
  cudaMemcpy(detector, cuda_detector, sizeof(float)*DET_SIZE*360, cudaMemcpyDeviceToHost);

  multiReconstruction<<< blocks, threads >>>(cuda_detector, cuda_rec_image);
  cudaThreadSynchronize();
  cudaMemcpy(rec_image, cuda_rec_image, sizeof(float)*IMAGE_SIZE*IMAGE_SIZE, cudaMemcpyDeviceToHost);

  cudaEventRecord(stop, 0);
  cudaEventSynchronize(stop);
  cudaEventElapsedTime(&time_ms, start, stop);
  printf("time: %f ms\n",time_ms);
  cudaEventDestroy(start);
  cudaEventDestroy(stop);

  fp = fopen("./cuda_image/cuda_inter_circle_detection_150-360.raw", "wb");
  fwrite(detector, sizeof(float),DET_SIZE*360,fp);
  fclose(fp);

  fp = fopen("./cuda_image/cuda_inter_circle_100-100.raw", "wb");
  fwrite(rec_image, sizeof(float),IMAGE_SIZE*IMAGE_SIZE,fp);
  fclose(fp);

  free(image);
  free(rec_image);
  free(detector);

  cudaFree(cuda_image);
  cudaFree(cuda_detector);
  cudaFree(cuda_rec_image);


}
