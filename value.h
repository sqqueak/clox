#ifndef clox_value_h
#define clox_value_h

#include "common.h"

typedef enum {
  VAL_BOOL,
  VAL_NIL,
  VAL_NUMBER,
} ValueType;

// tagged union between a 1-byte bool and a 8-byte double
typedef struct {
  ValueType type;   // type tag
  union {
    bool boolean;
    double number;
  } as;             // union data structure called "as"
} Value;

// type checkers to avoid using AS_ on the wrong type
#define IS_BOOL(value)    ((value).type == VAL_BOOL)
#define IS_NIL(value)     ((value).type == VAL_NIL)
#define IS_NUMBER(value)  ((value).type == VAL_NUMBER)

// unpack the union type to get the values
#define AS_BOOL(value)    ((value).as.boolean) // access bool in union
#define AS_NUMBER(value)  ((value).as.number)  // access num in union

// macros to create Value objects
#define BOOL_VAL(value)   ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL           ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})

// dynamic array holding values
typedef struct {
  int count;
  int capacity;
  Value* values;
} ValueArray;

void initValueArray(ValueArray* array);
void freeValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void printValue(Value value);

#endif