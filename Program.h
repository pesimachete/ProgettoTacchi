#ifndef PROGRAM_H
#define PROGRAM_H

#include "Block.h"

// Forward declaration of the Visitor class
// To avoid infinite inclusion
class Visitor;

class Program
{
public:
	Program(Block* MB) : MainBlock{ MB } {}
	// Deallocation of the block is the responsibility of the BlockManager, not the Program
	~Program() = default;

	Block* getBlock() {
		return MainBlock;
	}
	
	void accept(Visitor* v);
private:
	// A program has only one possible derivation, which is a block named MainBlock
	Block* MainBlock;
};
#endif
