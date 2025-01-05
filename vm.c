#include <stdarg.h>
#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "compiler.h"
#include "vm.h"

VM vm;

static void resetStack() {
  vm.stackTop = vm.stack; // put the stack pointer back at the top
}

// error reporting function with variable number of args?
static void runtimeError(const char* format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputs("\n", stderr);

  size_t instruction = vm.ip - vm.chunk->code - 1;
  int line = vm.chunk->lines[instruction];
  fprintf(stderr, "[line %d] in script\n", line);
  resetStack();
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

// look at a value some distance away from the top of the stack
static Value peek(int distance) {
  return vm.stackTop[-1 - distance];
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
// check if the two values at the top of the stack are numbers
#define BINARY_OP(valueType, op) \
  do { \
    if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
      runtimeError("Operands must be numbers."); \
      return INTERPRET_RUNTIME_ERROR; \
    } \
    double b = AS_NUMBER(pop()); \
    double a = AS_NUMBER(pop()); \
    push(valueType(a op b)); \
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
      case OP_ADD:      BINARY_OP(NUMBER_VAL, +); break;
      case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -); break;
      case OP_MULTIPLY: BINARY_OP(NUMBER_VAL, *); break;
      case OP_DIVIDE:   BINARY_OP(NUMBER_VAL, /); break;
      case OP_NEGATE: 
        if (!IS_NUMBER(peek(0))) {
          runtimeError("Operand must be a number.");
          return INTERPRET_RUNTIME_ERROR;
        }
        push(NUMBER_VAL(-AS_NUMBER(pop())));
        break;
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