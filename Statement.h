#ifndef STATEMENT_H
#define STATEMENT_H

#include "BoolExpr.h"
#include "NumExpr.h"
#include "Block.h"

// Forward declaration of the Visitor class
// To avoid infinite inclusion
class Visitor;

class Statement
{
public:
	
	// All classes derived from Statement must accept the visitor and implement a destructor.
	// Destructors are defaulted as the deallocation isn't the nodes' responsibility.
	// Allocation and deallocation will be handled by the respective Managers.
	virtual ~Statement() {};	
	
	virtual void accept(Visitor* v) = 0;
};
 
// The private attributes (immutable objects) are due to the derivations defined in the language grammar
// (Composite + Interpreter)
class PrintStmt : public Statement {
public:

	PrintStmt(NumExpr* n) : Printer{ n } { }

	~PrintStmt() = default;

	void accept(Visitor* v) override;

	// Access method
	NumExpr* getPrinter() const {
		return Printer;
	}

private:
	NumExpr* Printer; // The numerical expression to be printed.
};

class SetStmt : public Statement {
public:

	SetStmt(Variable* v, NumExpr* n) : var_id{ v }, Setter{ n } { }

	~SetStmt() = default;

	void accept(Visitor* v) override;

	// Access methods
	Variable* getVar() const {
		return var_id;
	}

	NumExpr* getSetter() const {
		return Setter;
	}

private:
	Variable* var_id; // The variable to be set.
	NumExpr* Setter; // The numerical expression used for setting.
};

class InputStmt : public Statement {
public:

	InputStmt(Variable* v): var_id{ v } { }

	~InputStmt() = default;

	void accept(Visitor* v) override;

	// Access method
	Variable* getVar() const {
		return var_id;
	}

private:
	Variable* var_id; // The variable for input.
};

class WhileStmt: public Statement {
public:

	WhileStmt(BoolExpr* b, Block* bb) : Condition{ b }, Reppeter{bb} {}

	~WhileStmt() = default;

	void accept(Visitor* v) override;

	// Access methods
	BoolExpr* getCondition() const {
		return Condition;
	}

	Block* getReppeter() const {
		return Reppeter;
	}

private:
	BoolExpr* Condition; // The boolean condition for looping.
	Block* Reppeter; // The block to be repeated.
};

class IfStmt : public Statement {
public:

	IfStmt(BoolExpr* c, Block* bi, Block* be) : Condition{ c }, IfBlock{ bi }, ElseBlock { be } {}

	~IfStmt() = default;

	void accept(Visitor* v) override;

	// Access methods
	BoolExpr* getCondition() const {
		return Condition;
	}

	Block* getIfBlock() const {
		return IfBlock;
	}

	Block* getElseBlock() const {
		return ElseBlock;
	}
private:
	BoolExpr* Condition; // The boolean condition for branching.
	Block* IfBlock;      // The block executed if the condition is true.
	Block* ElseBlock;    // The block executed if the condition is false.
};
#endif 