#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "compiler.h"
#include "vm.h"

VM vm;

static void resetStack() {
  vm.stackTop = vm.stack; // put the stack pointer back at the top
}

void initVM() {
  resetStack();
}

void freeVM() {
}

// pushes a value onto the vm's stack
void push(Value value) {
  *vm.stackTop = value; // stores value at top of stack
  vm.stackTop++; // advances pointer
}

// pops a value from the vm's stack
Value pop() {
  vm.stackTop--;
  return *vm.stackTop;
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
  do { \
    double b = pop(); \
    double a = pop(); \
    push(a op b); \
  } while (false);
  // using the do-while loop is to ensure that the macro is bound in the right
  // scope when executing it and allow a semicolon for compiler checks

  // read and execute bytecode instructions one by one
  for(;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("          ");
    // iterate through stack and display
    for(Value* slot = vm.stack; slot < vm.stackTop; slot++) {
      printf("[ ");
      printValue(*slot);
      printf(" ]");
    }
    printf("\n");
    disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif

    uint8_t instruction;
    switch(instruction = READ_BYTE()) {
      case OP_CONSTANT: {
        Value constant = READ_CONSTANT();
        push(constant);
        break;
      }
      case OP_ADD: BINARY_OP(+); break;
      case OP_SUBTRACT: BINARY_OP(-); break;
      case OP_MULTIPLY: BINARY_OP(*); break;
      case OP_DIVIDE: BINARY_OP(/); break;
      case OP_NEGATE: push(-pop()); break;
      case OP_RETURN: {
        printValue(pop());
        printf("\n");
        return INTERPRET_OK;
      }
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

// executes a chunk line by line
InterpretResult interpret(const char* source) {
  Chunk chunk;
  initChunk(&chunk);

  // attempt to compile source code into a chunk
  if (!compile(source, &chunk)) {
    freeChunk(&chunk);
    return INTERPRET_COMPILE_ERROR;
  }

  // set up vm to run chunk
  vm.chunk = &chunk;
  vm.ip = vm.chunk->code;

  // run chunk
  InterpretResult result = run();

  freeChunk(&chunk);
  return result;
}