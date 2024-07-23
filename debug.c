#include <stdio.h>

#include "debug.h"

/// @brief Prints each instruction in a Chunk.
/// @param chunk Chunk to be disassembled
/// @param name Name of Chunk
void disassembleChunk(Chunk* chunk, const char* name) {
	// Header denoting which Chunk is being looked at
	printf("== %s ==\n", name);

	for(int offset = 0; offset < chunk->count;) {
		// Prints each instruction
		offset = disassembleInstruction(chunk, offset);
	}
}

/// @brief 
/// @param name 
/// @param offset 
/// @return 
static int simpleInstruction(const char* name, int offset) {
	printf("%s\n", name);
	return offset + 1;
}

/// @brief Prints instruction based on which opcode is read.
/// @param chunk Chunk holding the instruction
/// @param offset Byte offset of instruction
/// @return Byte offset for next instruction
int disassembleInstruction(Chunk* chunk, int offset) {
	printf("%04d ", offset);

	uint8_t instruction = chunk->code[offset];
	// Branching for each possible opcode
	switch(instruction) {
		case OP_RETURN:
			return simpleInstruction("OP_RETURN", offset);
		default:
			printf("Unknown opcode %d\n", instruction);
			return offset + 1;
	}
}