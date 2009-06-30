#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define ARRAY_SIZE 128
#define UNIT unsigned short

int main(void) {
    int i;
    UNIT ar[ARRAY_SIZE];
    for (i=0; i<ARRAY_SIZE; i++) {
        ar[i]=i;
    }
    char* src_array = (char*) ar;
    int n_chars = ARRAY_SIZE * sizeof(UNIT);
    char* dst_array = malloc(n_chars);
    
    FILE *out_file = fopen("out.data", "w");
    memcpy(dst_array, src_array, n_chars);

    for (i=0; i<n_chars; i++) {
        fputc(dst_array[i], out_file);
    }

    return 0;
}

