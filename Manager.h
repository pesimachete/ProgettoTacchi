#ifndef MANAGER_H
#define MANAGER_H

#include<string>
#include<vector>

#include "Program.h"
#include "Block.h"
#include "BoolExpr.h"
#include "NumExpr.h"
#include "Statement.h"

// Managers are used to keep track of all allocated objects.
class Manager
{
public: 
	// The default constructor generates empty vectors initially, which is perfectly fine for my needs.
	Manager() = default;

	// Each of the derived classes will have to create its own memory deallocation method.
	virtual ~Manager() {};

	// I need to delete the copy and assignment methods to avoid ownership problems with pointers.
	Manager(const Manager& other) = delete;
	Manager& operator=(const Manager& other) = delete;

	// Template function to clear memory of a vector of objects.
	template <typename T>
	void clearMemory(std::vector<T>& vec);
};

// Managers are used to keep track of all allocated objects.
// Each of the derived classes will have to create its own memory deallocation method.


// Manages the allocation of numerical expressions.
class NumExprManager : public Manager{
public:
	// Create an operator numerical expression.
	NumExpr* makeOperator(Operator::OpCode op, NumExpr* l, NumExpr* r) {
		NumExpr* created = new Operator(op, l, r);
		NEallocated.push_back(created);
		return created;
	} 
	// Create a numerical constant.
	NumExpr* makeNumber(int value) {
		NumExpr* created = new Number(value);
		NEallocated.push_back(created);
		return created;
	}
	// Create a variable_id.
	NumExpr* makeVariable(std::string name) {
		NumExpr* created = new Variable(name);
		NEallocated.push_back(created);
		return created;
	}

	// Destructor to free allocated memory.
	 ~NumExprManager() override;
private: 
	std::vector<NumExpr*> NEallocated; // Stores pointers to allocated NumExpr objects.
};


// Manages the allocation of blocks.
class BlockManager : public Manager {
public:
	// Blocks are non - constant objects (at the time of allocation, it is not known which statements will be present).
	// Therefore, no parameters are needed to create it.
	Block* makeBlock() {
		Block* created = new Block();
		Ballocated.push_back(created);
		return created;
	}
	// Destructor to free allocated memory.
	 ~BlockManager() override;
private:
	std::vector<Block*> Ballocated; // Stores pointers to allocated Block objects.
};

// Manages the allocation of boolean expressions.
class BoolExprManager : public Manager {
public: 
	// Create a boolean constant.
	BoolExpr* makeBoolConst(bool b) {
		BoolExpr* created = new BoolConst(b);
		BEallocated.push_back(created);
		return created;
	}
	// Create a relational operator boolean expression.
	BoolExpr* makeRelOp(RelOp::RelOpCode o, NumExpr* lop, NumExpr* rop) {
		BoolExpr* created = new RelOp(o,lop,rop);
		BEallocated.push_back(created);
		return created;
	}
	// Create a boolean operator boolean expression with two operands.
	BoolExpr* makeBoolOp(BoolOp::BoolOpCode o, BoolExpr* lop, BoolExpr* rop) {
		BoolExpr* created = new BoolOp(o,lop,rop);
		BEallocated.push_back(created);
		return created;
	}
	// Create and manage a boolean operator boolean expression with one operand.(NOT)
	BoolExpr* makeBoolOp(BoolOp::BoolOpCode o, BoolExpr* lop) {
		BoolExpr* created = new BoolOp(o, lop);
		BEallocated.push_back(created);
		return created;
	}
	// Destructor to free allocated memory.
	~BoolExprManager() override;
private:
	std::vector<BoolExpr*> BEallocated; // Stores pointers to allocated BoolExpr objects.
};

// Manages the allocation of statements.
class StatementManager : public Manager {
public:
	// Create an if statement
	Statement* makeIfStmt(BoolExpr* c, Block* bi, Block* be) {
		Statement* created = new IfStmt(c,bi,be);
		Sallocated.push_back(created);
		return created;
	}
	// Create a while statement
	Statement* makeWhileStmt(BoolExpr* b, Block* bb) {
		Statement* created = new WhileStmt(b,bb);
		Sallocated.push_back(created);
		return created;
	}
	// Create an input statement
	Statement* makeInputStmt(Variable* v) {
		Statement* created = new InputStmt(v);
		Sallocated.push_back(created);
		return created;
	}
	// Create a print statement
	Statement* makePrintStmt(NumExpr* n) {
		Statement* created = new PrintStmt(n);
		Sallocated.push_back(created);
		return created;
	}
	// Create a set statement
	Statement* makeSetStmt(Variable* v, NumExpr* n) {
		Statement* created = new SetStmt(v,n);
		Sallocated.push_back(created);
		return created;
	}
	// Destructor to free allocated memory.
	~StatementManager() override;
private:
	std::vector<Statement*> Sallocated; // Stores pointers to allocated Statement objects.
};

// Manages the allocation of programs.
class ProgramManager : public Manager {
public:
	// Create a program.
	Program* makeProgram(Block* bb) {
		Program* created = new Program(bb);
		Pallocated.push_back(created);
		return created;
	}
	// Destructor to free allocated memory.
	~ProgramManager() override;
private:
	std::vector<Program*> Pallocated; // Stores pointers to allocated Program objects.
};

#endif