void image_malloc_one_dimension(unsigned char *image_arry, int image_size_bertical, int image_size_horizontal){
  image_arry = (unsigned char*)malloc(sizeof(unsigned char)*image_size_bertical*image_size_horizontal);
}

void image_malloc_two_dimension(unsigned char **image_arry, int image_size_bertical, int image_size_horizontal){
  image_arry = malloc(sizeof(unsigned char)*image_size_bertical);
  for(int i=0; i<image_size_bertical; i++){
    image_arry[i] = malloc(sizeof(unsigned char)*image_size_horizontal);
  }
}
