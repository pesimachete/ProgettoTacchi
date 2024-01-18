#ifndef BOOLEXPR_H
#define BOOLEXPR_H

#include "NumExpr.h"
#include "Exceptions.h"
#include "token.h"

// Forward declaration of the Visitor class
// To avoid infinite inclusion
class Visitor;



class BoolExpr
{
public: 
	// All classes derived from BoolExpr must accept the visitor and implement a destructor.
	virtual void accept(Visitor* v) = 0;
	virtual ~BoolExpr() {};
};

// The private attributes (immutable objects) are due to the derivations defined in the language grammar
// (Composite + Interpreter)

class RelOp : public BoolExpr{
public:
	// OpCode is responsible for identifying the relation operation to be performed with the operands.
	// It must be defined immutably during node creation.
	enum RelOpCode { LT, GT, EQ };

	~RelOp() = default;

	RelOp(RelOpCode o, NumExpr* lop, NumExpr* rop) : Cod{ o }, Left{ lop }, Right{ rop } { }

	// Static method to convert token tag to RelOpCode.
	static RelOpCode tokenTorelopcode(int tag) {
		switch (tag)
		{
		case token::LT: return LT; break;
		case token::GT: return GT; break;
		case token::EQ: return EQ; break;
		default:
			throw ConversionError("Token to RelOpCode conversion error");
			break;
		}
	}

	void accept(Visitor* v) override;

	RelOpCode getRelOpCode() const{
		return Cod;
	}

	NumExpr* getLeft() const{
		return Left;
	}

	NumExpr* getRight() const{
		return Right;
	}

private:
	RelOpCode Cod;
	NumExpr* Left;
	NumExpr* Right;
};

class BoolConst : public BoolExpr {
public:

	BoolConst(bool b) : boolvalue{ b } {}

	~BoolConst() = default;

	// Static method to convert token tag to boolean value.
	static bool tokenTobool(int tag) {
		bool b;
		switch (tag){
		case 16:
			b = true;
			break;
		case 17:
			b = false;
			break;
		default:
			throw ConversionError("Token to Bool conversion error");
			break;
		}
		return b;
	}

	void accept(Visitor* v) override;
	
	bool getValue() {
		return boolvalue;
	}

private:
	bool boolvalue;
};

class BoolOp: public BoolExpr {
public:

	enum BoolOpCode { AND, OR, NOT };

	// Constructor: Initializes a BoolOp with a boolean operation code and left and right boolean expressions.
	BoolOp(BoolOpCode o, BoolExpr* lop, BoolExpr* rop) : bop{ o }, Left{ lop }, Right{ rop } { }

	// Constructor for unary boolean operations.
	BoolOp(BoolOpCode o, BoolExpr* lop) : bop{ o }, Left{ lop }, Right{ nullptr } { }

	void accept(Visitor* v) override;

	// Static method to convert token tag to BoolOpCode.
	static BoolOpCode tokenToboolopcode(int tag) {
		switch (tag)
		{
		case token::AND: return AND; break;
		case token::OR: return OR; break;
		case token::NOT: return NOT; break;
		default:
			throw ConversionError("Token to BoolOpCode conversion error");
			break;
		}
	}

	BoolOpCode getBoolOpCode() {
		return bop;
	}

	BoolExpr* getLeft() {
		return Left;
	}

	BoolExpr* getRight() {
		return Right;
	}


private:
	BoolOpCode bop;
	BoolExpr* Left;
	BoolExpr* Right;

};
#endif