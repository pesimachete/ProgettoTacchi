#ifndef NUMEXPR_H
#define NUMEXPR_H

#include <string> 

// Forward declaration of the Visitor class
// To avoid infinite inclusion
class Visitor;


// All objects of type NumExpr are immutable and created using the Composite + Interpreter design pattern.
class NumExpr
{
public:
	// All classes derived from NumExpr must accept the visitor and implement a destructor.
	virtual ~NumExpr() {};
	virtual void accept(Visitor* v) = 0;
};
 
 
class Operator : public NumExpr {
public:
	// OpCode is responsible for identifying the operation to be performed with the operands.
	// It must be defined immutably during node creation.
	enum OpCode { ADD, SUB, MUL, DIV };

	Operator(OpCode o, NumExpr* le, NumExpr* ri) : Cod {o}, Left{le}, Right{ri} { }

	// Destructor is defaulted as the deallocation isn't the node's responsibility.
	// Allocation and deallocation will be handled by the NumExprManager.
	~Operator() = default;

	void accept(Visitor* v) override;

	OpCode getOpCode() const{
		return Cod;
	}

	NumExpr* getLeft() const{
		return Left;
	}

	NumExpr* getRight() const{
		return Right;
	}

private:
	OpCode Cod;
	NumExpr* Left; 
	NumExpr* Right;
};

class Number : public NumExpr {
public:
	Number(long int v) : intValue{ v } { }

	~Number() = default;

	void accept(Visitor* v) override;

	long int getValue() const{
		return intValue;
	}

private:
	long int intValue;
};

class Variable : public NumExpr {
public:

	Variable(std::string v): variable_id {v} {}

	~Variable() = default;

	void accept(Visitor* v) override;

	std::string getVarId() const{
		return variable_id;
	}
private:
	std::string variable_id;
};

#endif 
