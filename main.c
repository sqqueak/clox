#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

// reads code from terminal in a loop
static void repl() {
  char line[1024];
  for(;;) {
    printf("> ");

    if(!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }

    interpret(line);
  }
}

// reads code from a file
static char* readFile(const char* path) {
  FILE* file = fopen(path, "rb");
  if(file == NULL) {
    fprintf(stderr, "Could not open file \"%s\".\n", path);
    exit(74);
  }

  fseek(file, 0L, SEEK_END); // point to end of file
  size_t fileSize = ftell(file); // gets num of bytes from start of file
  rewind(file); // go to beginning

  char* buffer = (char*)malloc(fileSize + 1); // alloc string of right size
  if(buffer == NULL) {
    fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
    exit(74);
  }

  size_t bytesRead = fread(buffer, sizeof(char), fileSize, file); // read file
  if(bytesRead < fileSize) {
    fprintf(stderr, "Could not read file \"%s\".\n", path);
    exit(74);
  }

  buffer[bytesRead] = '\0'; // set term char

  fclose(file);
  return buffer;
}

// mem + error management for file read
static void runFile(const char* path) {
  char* source = readFile(path);
  InterpretResult result = interpret(source);
  free(source);

  if(result == INTERPRET_COMPILE_ERROR) exit(65);
  if(result == INTERPRET_RUNTIME_ERROR) exit(70);
}

int main(int argc, const char* argv[]) {
  initVM();

  if(argc == 1) {
    repl();
  } else if(argc == 2) {
    runFile(argv[1]);
  } else {
    fprintf(stderr, "Usage: clox [path]\n");
    exit(64);
  }

  return 0;
}