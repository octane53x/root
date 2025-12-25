// MALLOC TEST

#include <stdio.h>
#include <stdlib.h>

int main(){
  size_t size = 1024; // 1 KB
  while(1){
    void* mem = malloc(size);
    if(mem == NULL){
      printf("Failed to allocate %lf MB\n", (double)size / (1024 * 1024));
      break;
    }else
      printf("Successfully allocated %lf MB\n", (double)size / (1024 * 1024));
    free(mem);
    size <<= 1; }
  return 0; }
