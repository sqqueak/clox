#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"

// Calculates a new capacity given current capacity
#define GROW_CAPACITY(capacity) \
  ((capacity) < 8 ? 8 : (capacity) * 2)

// Creates new array and moves pointer to point at new array
#define GROW_ARRAY(type, pointer, oldCount, newCount) \
  (type*)reallocate(pointer, sizeof(type) * (oldCount), \
    sizeof(type) * (newCount))

// Frees array by allocating array to size 0
#define FREE_ARRAY(type, pointer, oldCount) \
  reallocate(pointer, sizeof(type) * (oldCount), 0)

void* reallocate(void* pointer, size_t oldSize, size_t newSize);

#endif