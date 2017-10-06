#define IMAGE_SIZE 256

char *binary_code, *binary_code_inv;

unsigned char **raw_image;

int block_number, element_number, diff_value, dc_group, ac_group, bit_number, ac_zero_run;

int **quantized_image, **scaned_quantized_image_arry;

double **dct_image, **integral_image_cosine;

FILE *fp;
