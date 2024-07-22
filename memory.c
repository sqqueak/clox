#include <stdlib.h>

#include "memory.h"

/// @brief Reallocates memory so the pointer moves from pointing to an array of
/// oldSize to an array of newSize.
/// @param pointer Pointer to memory block
/// @param oldSize Previous size of memory block
/// @param newSize Expected size of memory block
/// @return Pointer to resized memory block
void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
    // Free allocation
    if(newSize == 0) {
        free(pointer);
        return NULL;
    }

    // Deals with allocating new memory and growing and shrinking current memory
    void* result = realloc(pointer, newSize);
    if(result == NULL) exit(1); // realloc failed
    return result;
}