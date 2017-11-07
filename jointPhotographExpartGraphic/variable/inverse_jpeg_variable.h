#include <stdbool.h>

// unsigned char **inversed_image;
int inverse_binary_progress, ac_vlc, dc_value, ac_value, pre_dc_value;
int *inversed_encoded_binary_array, **inversed_frequency_component_arry;

long inverse_binary_number;

//逆変換の時に使うフラグ
bool end_block_flag, special_bit_flag, dc_flag = true;

//256配列
unsigned char **inv_raw_image;
double **quantized_image_inverse;
int **scaned_freaquency_image;

//ビットで利用する配列
int **inversed_frequency_component_arry, *inversed_encoded_binary_array;
