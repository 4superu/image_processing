#include <stdbool.h>

int ac_vlc, dc_value, ac_value, pre_dc_value;

int *inversed_encoded_binary_array, **inversed_frequency_component_arry;

long inverse_binary_number, inverse_binary_progress;

//逆変換の時に使うフラグ
bool end_block_flag, special_bit_flag, dc_flag = true;

//256配列
double **inv_raw_image;
double **quantized_image_inverse;
int **scaned_freaquency_image;

//ビットで利用する配列
int **inversed_frequency_component_arry, *inversed_encoded_binary_array;

unsigned char **normalized_inv_image;
