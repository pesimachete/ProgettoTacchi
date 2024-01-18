#include "Block.h"
#include "Visitor.h"

// Allowing it to accept the visitor by invoking the visit to the block.

void Block::accept(Visitor* v)
{
	v->visitBlock(this);
}
