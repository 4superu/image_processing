#include <stdbool.h>

double return_coefficient(int u, int v){

  switch (u) {

    case 0:
    switch (v) {
      case 0:
      return 1.0/2.0;

      default:
      return 1.0/sqrt(2.0);

    }

    default:
    switch (v) {
      case 0:
      return 1.0/sqrt(2.0);

      default:
      return 1.0;
    }

  }

}

char dc_encode_hahuman(int block_number, double **scaned_quantized_image_arry){
  int i=0, dc_group;
  char hahuman_code;
  double difference_dc_elsement;

  if(block_number == 0){
    difference_dc_elsement = log2(fabs(scaned_quantized_image_arry[block_number][0] - 0));
  }
  else{
    difference_dc_elsement = log2(fabs(scaned_quantized_image_arry[block_number][0] - scaned_quantized_image_arry[block_number-1][0]));
  }
  if(difference_dc_elsement < 1.0){
    dc_group = 0;
  }
  else{
    dc_group = (int)foor(difference_dc_elsement+1.0)
  }

  while(dc_code_table[dc_group][i] == -1){
    hahuman_code += dc_code_table[dc_group][i];
    i++;
  }
  return hahuman_code;
}

// int ac_encode_hahuman(int block_number, int element_number, double **scaned_quantized_image_arry){
//
// }
