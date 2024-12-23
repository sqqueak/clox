#include <stdlib.h>

#include "memory.h"

// realloc space to the pointer from the old size to the new size
void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
  // check if need to free
  if(newSize == 0) {
    free(pointer);
    return NULL;
  }
  // call internal realloc
  void* result = realloc(pointer, newSize);
  if(result == NULL) exit(1);
  return result;
}