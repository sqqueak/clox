#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

/// @brief Initializes a Chunk.
/// @param chunk Pointer to chunk to initialize
void initChunk(Chunk* chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
}

/// @brief Frees a Chunk.
/// @param chunk Pointer to chunk to free
void freeChunk(Chunk* chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  initChunk(chunk); // Zeros out the fields
}

/// @brief Appends a new value to the end of a Chunk.
/// @param chunk Pointer to chunk to write to
/// @param byte Value to be written
void writeChunk(Chunk* chunk, uint8_t byte) {
  // Check if the array is full, if so, expand it
  if(chunk->capacity < chunk->count + 1) {
    // Increase the capaicty and copy data from old array into new array.
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code = GROW_ARRAY(
      uint8_t, chunk->code, oldCapacity, chunk->capacity);
  }

  // Append new item onto the end of the array
  chunk->code[chunk->count] = byte;
  chunk->count++;
}
