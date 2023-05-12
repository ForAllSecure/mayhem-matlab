#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mayhemit.h" // Header file generated by MATLAB Coder
#include "mayhemit_emxAPI.h"
#include "mayhemit_emxutil.h"
#include "mayhemit_types.h"


emxArray_char_T* convert_char_to_emxArray_char_T(char* input_str) {
    int rows = 1; // Assuming a single row
    int cols = strlen(input_str);

    emxArray_char_T* emx_arr = emxCreateWrapper_char_T(input_str, rows, cols);
    return emx_arr;
}


int main(int argc, char *argv[]) {
  FILE *f;
  char buf[12];

  if(argc != 2){
    fprintf(stderr, "Must supply a text file\n");
    return -1;
  }
  f = fopen(argv[1], "r");
  if(f == NULL){
    fprintf(stderr, "Could not open %s\n", argv[1]);
    return -1;
  }
  if(fgets(buf, sizeof(buf), f) == NULL){
    fprintf(stderr, "Could not read from %s\n", argv[1]);
    return -1;
  }
  emxArray_char_T* emx_arr = convert_char_to_emxArray_char_T(buf);
  mayhemit(emx_arr);
  emxDestroyArray_char_T(emx_arr);
  return 0;
}