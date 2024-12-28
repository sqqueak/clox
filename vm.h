#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"

#define STACK_MAX 256

// a virtual machine consists of a series of chunks of code
typedef struct {
  Chunk* chunk;
  uint8_t* ip;
  Value stack[STACK_MAX];
  Value* stackTop;
} VM;

// possible completion states after interpreting a chunk
typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(Chunk* chunk);
void push(Value value);
Value pop();

#endif