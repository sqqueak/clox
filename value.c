#include <stdio.h>

#include "memory.h"
#include "value.h"

// initialize array to default values
void initValueArray(ValueArray* array) {
  array->count = 0;
  array->capacity = 0;
  array->values = NULL;
}

// dealloc and zero out array memory
void freeValueArray(ValueArray* array) {
  FREE_ARRAY(Value, array->values, array->capacity);
  initValueArray(array);
}

// adds value to array, expanding array size if necessary
void writeValueArray(ValueArray* array, Value value) {
  // check if at capacity, if so, expand array
  if(array->capacity < array->count + 1) {
    int oldCapacity = array->capacity;
    array->capacity = GROW_CAPACITY(oldCapacity);
    array->values = GROW_ARRAY(
        Value, array->values, oldCapacity, array->capacity);
  }
  // add new element to array
  array->values[array->count] = value;
  array->count++;
}

// prints out a value!
void printValue(Value value) {
  switch (value.type) {
    case VAL_BOOL: printf(AS_BOOL(value) ? "true" : "false"); break;
    case VAL_NIL: printf("nil"); break;
    case VAL_NUMBER: printf("%g", AS_NUMBER(value)); break;
  }
}