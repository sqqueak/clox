#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"

/// @brief Specifies the possible instructions (opcodes) we can have.
typedef enum {
  OP_RETURN,
} OpCode;

/// @brief Represents a dynamic array.
typedef struct {
  int count;      // Total elements stored in the array
  int capacity;   // Maximum number of elements that can be stored
  uint8_t* code;  // Pointer to array that stores the values
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte);

#endif