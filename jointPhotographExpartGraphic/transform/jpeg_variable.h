#define IMAGE_SIZE 256

char *binary_code_inv;

unsigned char **raw_image, *raw_encode_data;

int block_number, element_number, dc_diff_value, ac_value, dc_group, ac_group, bit_number, ac_zero_run;

long encode_data_number;

int **quantized_image, **scaned_quantized_image_arry, *encode_data_arry, *binary_code;

double **dct_image, **integral_image_cosine;

FILE *fp;
