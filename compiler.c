#include <stdio.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

void compile(const char* source) {
  initScanner(source);

  // tmp scanner code :D
  // prints out each token in a line of code
  int line = -1;
  for(;;) {
    Token token = scanToken();
    if(token.line != line) {
      printf("%4d ", token.line);
      line = token.line;
    } else {
      printf("   | ");
    }
    printf("%2d '%.*s'\n", token.type, token.length, token.start); 

    if(token.type == TOKEN_EOF) break;
  }
}