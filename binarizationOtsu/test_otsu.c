#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(){

	int i, j;
  int left_pixel, right_pixel, left_class_value, right_class_value, threshould_value;
	double *data, left_class_ave, right_class_ave, threshould_degree, max_threshould_degree;
	int *data_histgram;

  data = malloc(sizeof(double)*10000);
  data_histgram = malloc(sizeof(int)*100);

	srand((unsigned)time(NULL));

  //初期化
	for(i=0; i<10000; i++){
		data[i] = 0;
	}
	for(i = 0; i < 100 ; i++){
		data_histgram[i] = 0;
	}

	for(i=0; i<10000; i++){
		data[i] = (rand() / (RAND_MAX + 1.0)) * 100;
		data_histgram[(int)(data[i])]++;
	}

	for(i=1; i<100; i++){

    left_pixel = 0;
    right_pixel = 0;
    left_class_value = 0;
    right_class_value = 0;

		for(j=0; j<=i; j++){
			left_pixel += data_histgram[j];
			left_class_value += j * data_histgram[j];
		}

		for(j=i+1; j<100; j++){
			right_pixel += data_histgram[j];
			right_class_value += j * data_histgram[j];
		}

		left_class_ave = (double)left_class_value / (double)left_pixel;
		right_class_ave = (double)right_class_value / (double)right_pixel;

		threshould_degree = (double)left_pixel * right_pixel * pow((left_class_ave - right_class_ave), 2);

		if(threshould_degree > max_threshould_degree){
			max_threshould_degree = threshould_degree;
			threshould_value = i;
		}
	}

	printf("閾値：%d\n", threshould_value);

}
