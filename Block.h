#ifndef BLOCK_H
#define BLOCK_H

#include <vector>

// Forward declaration of the Visitor class and Statement class;
// To avoid infinite inclusion
class Statement;
class Visitor;

class Block
{
public:
	// To create a Block, no arguments are required; new statements can be added to the block using the pushback function
	Block() {};
	// The responsibility of deallocating Statements from the stmt_list lies with the StatementManager, not the Block
	~Block() = default;

	void pushback(Statement* s ) {
		stmt_list.push_back(s);
	}

	void accept(Visitor* v);

	std::vector<Statement*> getVector() const {
		return stmt_list;
	}
	
private:
	// A block is derived as a non-empty sequence of Statements
	std::vector<Statement*>  stmt_list;	
};
#endif

