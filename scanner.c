#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct {
  const char* start;
  const char* current;
  int line;
} Scanner;

Scanner scanner;

// sets scanner variables to the start
void initScanner(const char* source) {
  scanner.start = source; // start of current lexeme
  scanner.current = source; // character in current lexeme
  scanner.line = 1;
}

// checks [a-zA-Z_]
static bool isAlpha(char c) {
  return (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') ||
          c == '_';
}

// checks [0-9]
static bool isDigit(char c) {
  return c >= '0' && c <= '9';
}

// checks if current character is null
static bool isAtEnd() {
  return *scanner.current == '\0';
}

// consumes and returns next character
static char advance() {
  scanner.current++;
  return scanner.current[-1];
}

// returns next character
static char peek() {
  return *scanner.current;
}

// returns next-next character
static char peekNext() {
  if(isAtEnd()) return '\0';
  return scanner.current[1];
}

// checks if scanner's current char is as expected
static bool match(char expected) {
  if(isAtEnd()) return false;
  if(*scanner.current != expected) return false;
  scanner.current++;
  return true;
}

// creates token for current lexeme
static Token makeToken(TokenType type) {
  Token token;
  token.type = type;
  token.start = scanner.start;
  token.length = (int)(scanner.current - scanner.start);
  token.line = scanner.line;
  return token;
}

// creates an error token with the specified message
static Token errorToken(const char* message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int)strlen(message);
  token.line = scanner.line;
  return token;
}

// consumes whitespace characters and comments
static void skipWhitespace() {
  for(;;) {
    char c = peek();
    switch(c) {
      case ' ':
      case '\r':
      case '\t':
        advance();
        break;
      case '\n':
        scanner.line++;
        advance();
        break;
      case '/': // checking if comment
        if(peekNext() == '/') { 
          while(peek() != '\n' && !isAtEnd()) advance(); // scan til end of line
        } else {
          return;
        }
        break;
      default:
        return;
    }
  }
}

// checks if current lexeme is a reserved keyword
static TokenType checkKeyword(int start, int length, const char* rest, 
                              TokenType type) {
  // size of the keyword is the same as the position diff of the lexeme
  if(scanner.current - scanner.start == start + length) {
    // contents are as expected 
    if(memcmp(scanner.start + start, rest, length) == 0) {
      return type; // reserved keyword
    }
  }

  return TOKEN_IDENTIFIER; // normal identifier
}

// gets token type of an identifier or keyword
static TokenType identifierType() {
  switch(scanner.start[0]) {
    case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
    case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
    case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
    case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
    case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
    case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
    case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
    case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
    case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
    case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
    case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    case 'f':
      if (scanner.current - scanner.start > 1) {
        switch (scanner.start[1]) {
          case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
          case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
          case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
        }
      }
      break;
    case 't':
      if (scanner.current - scanner.start > 1) {
        switch (scanner.start[1]) {
          case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
          case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
        }
      }
      break;
  }
  
  return TOKEN_IDENTIFIER;
}

// creates identifier token
static Token identifier() {
  while(isAlpha(peek()) || isDigit(peek())) advance();
  return makeToken(identifierType());
}

// creates number token
static Token number() {
  while(isDigit(peek())) advance();

  if(peek() == '.' && isDigit(peekNext())) { // check for decimal point
    advance(); // consume decimal
    while(isDigit(peek())) advance();
  }

  return makeToken(TOKEN_NUMBER);
}

// creates string token
static Token string() {
  while(peek() != '"' && !isAtEnd()) { // haven't reached closing quote yet
    if(peek() == '\n') scanner.line++; // supports multiline strings
    advance();
  }

  if(isAtEnd()) return errorToken("Unterminated string.");

  advance(); // closing quote
  return makeToken(TOKEN_STRING);
}

Token scanToken() {
  skipWhitespace();
  scanner.start = scanner.current;

  if(isAtEnd()) return makeToken(TOKEN_EOF);

  char c = advance();
  if(isAlpha(c)) return identifier();
  if(isDigit(c)) return number();

  switch(c) {
    // single char tokens
    case '(': return makeToken(TOKEN_LEFT_PAREN);
    case ')': return makeToken(TOKEN_RIGHT_PAREN);
    case '{': return makeToken(TOKEN_LEFT_BRACE);
    case '}': return makeToken(TOKEN_RIGHT_BRACE);
    case ';': return makeToken(TOKEN_SEMICOLON);
    case ',': return makeToken(TOKEN_COMMA);
    case '.': return makeToken(TOKEN_DOT);
    case '-': return makeToken(TOKEN_MINUS);
    case '+': return makeToken(TOKEN_PLUS);
    case '/': return makeToken(TOKEN_SLASH);
    case '*': return makeToken(TOKEN_STAR);

    // one or two char tokens
    case '!': 
      return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=': 
      return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '<': 
      return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>': 
      return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);

    // literals
    case '"': return string();
  }

  return errorToken("Unexpected character.");
}