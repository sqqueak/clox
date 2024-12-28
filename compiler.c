#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

// stores the status of the parser and recent tokens
typedef struct {
  Token current;
  Token previous;
  bool hadError;
  bool panicMode;
} Parser;

Parser parser;

// displays error message at token's location
static void errorAt(Token* token, const char* message) {
  if (parser.panicMode) return; // skip if this is part of a cascade of errors
  parser.panicMode = true;
  fprintf(stderr, "[line %d] Error", token->line);

  if (token->type == TOKEN_EOF) {
    fprintf(stderr, " at end");
  } else if (token->type == TOKEN_ERROR) {
  } else {
    fprintf(stderr, " at '%.*s", token->length, token->start);
  }

  fprintf(stderr, ": %s\n", message);
  parser.hadError = true;
}

// error with previous token
static void error(const char* message) {
  errorAt(&parser.previous, message);
}

// error with current token
static void errorAtCurrent(const char* message) {
  errorAt(&parser.current, message);
}

// set parser status to next token
static void advance() {
  parser.previous = parser.current;

  for (;;) {
    parser.current = scanToken();
    if (parser.current.type != TOKEN_ERROR) break;

    errorAtCurrent(parser.current.start);
  }
}

// move to next token
static void consume(TokenType type, const char* message) {
  if (parser.current.type == type) {
    advance();
    return;
  }

  errorAtCurrent(message);
}

bool compile(const char* source, Chunk* chunk) {
  initScanner(source);

  parser.hadError = false;
  parser.panicMode = false;

  advance();
  expression();
  consume(TOKEN_EOF, "Expect end of expression.");
  return !parser.hadError;
}