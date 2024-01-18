#ifndef VISITOR_H
#define VISITOR_H

#include <iostream>
#include <string>
#include <vector>

#include "Exceptions.h"
#include "token.h"
#include "Program.h"
#include "Block.h"
#include "BoolExpr.h"
#include "NumExpr.h"
#include "Statement.h"
#include "SymbolTable.h"

// The Visitor class defines a visitor pattern for traversing the syntax tree.
// tutti i tipi di visite devo creare metodi che sono capaci de fare la visita ad ogniuno dai tipi di nodi presenti nel albero del programma 
// e tutti gli elementi hanno la funzione di accertare il visitor, tornando possibile la ultima parte del programma che è la sua interpretazione
class Visitor
{
public:
	// Visits the Program node.
	virtual void visitProgram(Program* progNode) = 0;

	// Visits the Block node.
	virtual void visitBlock(Block* blockNode) = 0;

	// Visits various statement nodes.
	virtual void visitPrintStmt(PrintStmt* printStmtNode) = 0;
	virtual void visitSetStmt(SetStmt* setStmtNode) = 0;
	virtual void visitInputStmt(InputStmt* inputStmtNode) = 0;
	virtual void visitWhileStmt(WhileStmt* whileStmtNode) = 0;
	virtual void visitIfStmt(IfStmt* ifStmtNode) = 0;

	// Visits various expression nodes.
	virtual void visitOperator(Operator* opNode) = 0;
	virtual void visitNumber(Number* numNode) = 0;
	virtual void visitVariable(Variable* varNode) = 0;

	// Visits relational operator and boolean nodes.
	virtual void visitRelOp(RelOp* relOpNode) = 0;
	virtual void visitBoolConst(BoolConst* boolConstNode) = 0;
	virtual void visitBoolOp(BoolOp* boolOpNode) = 0;
};

// The PrintVisitor class is an implementation of the Visitor interface that prints the syntax tree.
// Questa visita non è utile para il programma finale, è stato essenciale per il teste della creazione del albero sintatico
class PrintVisitor: public Visitor {
	void visitProgram(Program* progNode) {
		std::cout << "Inizio del programa: ";
		progNode->getBlock()->accept(this);
	}

	void visitBlock(Block* blockNode) {
		std::cout << "(BLOCK ";
		for (auto i : blockNode->getVector()) {
			i->accept(this);
		}
	}

	void visitPrintStmt(PrintStmt* printStmtNode) {
		std::cout << "(PRINT ";
		printStmtNode->getPrinter()->accept(this);
		std::cout << " ) ";
	}
	void visitSetStmt(SetStmt* setStmtNode) {
		std::cout << "(SET ";
		setStmtNode->getVar()->accept(this);
		std::cout << " ";
		setStmtNode->getSetter()->accept(this);
		std::cout << " ) ";
	}
	void visitInputStmt(InputStmt* inputStmtNode) {
		std::cout << "(INPUT ";
		inputStmtNode->getVar()->accept(this);
		std::cout << " ) ";
	}
	void visitWhileStmt(WhileStmt* whileStmtNode) {
		std::cout << "(WHILE ";
		whileStmtNode->getCondition()->accept(this);
		std::cout << " ";
		whileStmtNode->getReppeter()->accept(this);
		std::cout << " ) ";
	}
	void visitIfStmt(IfStmt* ifStmtNode) {
		std::cout << "(IF ";
		ifStmtNode->getCondition()->accept(this);
		std::cout << " ";
		ifStmtNode->getIfBlock()->accept(this);
		std::cout << " ";
		ifStmtNode->getElseBlock()->accept(this);
		std::cout << " ) ";
	}

	void visitOperator(Operator* opNode) {
		std::cout << " ( " << opNode->getOpCode() << " ";
		opNode->getLeft()->accept(this);
		std::cout << " ";
		opNode->getRight()->accept(this);
		std::cout << " ) ";
	}
	void visitNumber(Number* numNode) {
		std::cout << numNode->getValue();
	}
	void visitVariable(Variable* varNode) {
		std::cout << varNode->getVarId();
	}

	void visitRelOp(RelOp* relOpNode) {
		std::cout << " ( " << relOpNode->getRelOpCode() << " ";
		relOpNode->getLeft()->accept(this);
		std::cout << " ";
		relOpNode->getRight()->accept(this);
		std::cout << " ) ";
	}
	void visitBoolConst(BoolConst* boolConstNode) {
		std::cout << boolConstNode->getValue();
	}
	void visitBoolOp(BoolOp* boolOpNode) {
		std::cout << " ( " << boolOpNode->getBoolOpCode() << " ";
		boolOpNode->getLeft()->accept(this);
		std::cout << " ";
		boolOpNode->getRight()->accept(this);
		std::cout << " ) ";
	}
};

// The EvaluatorVisitor class is an implementation of the Visitor interface that evaluates the program with expressions and statements.
class EvaluatorVisitor :public Visitor {
public:
	EvaluatorVisitor(SymbolTable& S): ST{S} {}
	
	void visitProgram(Program* progNode) {
		// Start the evaluation by visiting the Program's Block.
		progNode->getBlock()->accept(this);
	}

	void visitBlock(Block* blockNode) {
		// Iterate through the statements within the Block and visit each one.
		for (auto i : blockNode->getVector()) {
			i->accept(this);
		}
	}

	void visitPrintStmt(PrintStmt* printStmtNode) {
		// Visit the expression to be printed and evaluate it
		printStmtNode->getPrinter()->accept(this);
		// Retrieve the result of the expression evaluation from the accumulator and print it
		int numPrintable = NumExprAccumulator.back(); NumExprAccumulator.pop_back();
		std::cout << numPrintable << std::endl;
	}
	void visitSetStmt(SetStmt* setStmtNode) {
		// Get the variable name to set
		std::string vi = setStmtNode->getVar()->getVarId();
		// Visit and evaluate the expression that provides the new value for the variable
		setStmtNode->getSetter()->accept(this);
		// Retrieve the result of the expression evaluation and update the variable's value in the symbol table
		int numSetter = NumExprAccumulator.back(); NumExprAccumulator.pop_back();
		ST.CCvar(vi, numSetter);
		return;
	}
	void visitInputStmt(InputStmt* inputStmtNode) {
		// Get the variable name to input a value into
		std::string vi = inputStmtNode->getVar()->getVarId();
		std::string stringInput;
		// Read a string input from the user
		std::cin >> stringInput;
		// Check if the input string is a valid numeric value
		for (size_t i = 0; i < stringInput.length(); ++i) {
			if (!isdigit(stringInput[i]) && !(i == 0 && stringInput[i] == '-')) {
				throw SemanticError("NOT A ACCETABLE NUMBER ");
				return;
			}
		}
		// Convert the input string to a long int and update the variable's value in the symbol table
		long int numInput = std::stoi(stringInput);
		ST.CCvar(vi, numInput);
		return;
	}
	void visitWhileStmt(WhileStmt* whileStmtNode) {
		// Evaluate the condition expression
		whileStmtNode->getCondition()->accept(this);
		// Retrieve the boolean result of the condition evaluation
		bool cond = BoolExprAccumulator.back(); BoolExprAccumulator.pop_back();
		// Execute the loop as long as the condition is true
		while (cond)
		{
			whileStmtNode->getReppeter()->accept(this);
			whileStmtNode->getCondition()->accept(this);
			cond = BoolExprAccumulator.back(); BoolExprAccumulator.pop_back();
		}
	}
	void visitIfStmt(IfStmt* ifStmtNode) {
		// Evaluate the condition expression
		ifStmtNode->getCondition()->accept(this);
		// Retrieve the boolean result of the condition evaluation
		bool cond = BoolExprAccumulator.back(); BoolExprAccumulator.pop_back();
		// Execute the if or else block based on the condition result
		if (cond) {
			ifStmtNode->getIfBlock()->accept(this);
		}
		else {
			ifStmtNode->getElseBlock()->accept(this);
		}
	}


	// Evaluation of numeric expressions or boolean expressions:
	// The evaluation is done in such a way that the resulting value of its evaluation is in the last position of the accumulator.
	// This way, whoever requested the visit to this number can find the result at the top of the accumulator.
	// After reading the value, it is removed from the accumulator.




	void visitOperator(Operator* opNode) {
		// Propagate the visit to the operands and retrieve the values
		opNode->getLeft()->accept(this);
		int lval = NumExprAccumulator.back(); NumExprAccumulator.pop_back();
		opNode->getRight()->accept(this);
		int rval = NumExprAccumulator.back(); NumExprAccumulator.pop_back();
		switch (opNode->getOpCode())
		{
		// Perform the arithmetic operation and store the result in the accumulator
		case Operator::ADD:
			NumExprAccumulator.push_back(lval + rval); return;
		case Operator::SUB:
			NumExprAccumulator.push_back(lval - rval); return;
		case Operator::MUL:
			NumExprAccumulator.push_back(lval * rval); return;
		case Operator::DIV:
			if (rval == 0) {
				// Handle division by zero error
				throw SemanticError("ZERO DIVISION");
			}
			NumExprAccumulator.push_back(lval / rval); return;
		default:
			// This error should not occur because it should have already been handled in previous stages
			throw SemanticError("INVALID operation");
			return;
		}
	}
	void visitNumber(Number* numNode) {
		// Store the value of the number in the accumulator
		NumExprAccumulator.push_back(numNode->getValue());
		return;
	}
	void visitVariable(Variable* varNode) {
		// Visiting a variable retrieves its value and puts it in the accumulator.
		// The creation and modification of variable values can only be done through a set or input statement,
		// which do not call the visit to the variable, but rather use the symbol table with CCvar.
		NumExprAccumulator.push_back(
			ST.getValueFromVariable(
				varNode->getVarId()
			));
		return;
	}



	void visitRelOp(RelOp* relOpNode) {
		// Propagate the visit to the operands and retrieve the values
		relOpNode->getLeft()->accept(this);
		int lval = NumExprAccumulator.back(); NumExprAccumulator.pop_back();
		relOpNode->getRight()->accept(this);
		int rval = NumExprAccumulator.back(); NumExprAccumulator.pop_back();
		switch (relOpNode->getRelOpCode())
		{
		// Perform the relational operation and store the result in the accumulator.
		case RelOp::EQ:
			BoolExprAccumulator.push_back(lval == rval); return;
		case RelOp::LT:
			BoolExprAccumulator.push_back(lval < rval); return;
		case RelOp::GT:
			BoolExprAccumulator.push_back(lval > rval); return;
		default:
			// This error should not occur because it should have already been handled in previous stages
			throw SemanticError("INVALID realtional operator");
			return;
		}
	}
	void visitBoolConst(BoolConst* boolConstNode) {
		// Store the boolean constant value in the accumulator
		BoolExprAccumulator.push_back(boolConstNode->getValue()); return;
	}
	void visitBoolOp(BoolOp* boolOpNode) {
		// Evaluate the left operand
		boolOpNode->getLeft()->accept(this);
		bool lval = BoolExprAccumulator.back(); BoolExprAccumulator.pop_back();
		if (boolOpNode->getBoolOpCode() == BoolOp::AND && lval == false) {
			// Short-circuit AND operation (if left operand is false, result is false)
			BoolExprAccumulator.push_back(false);  return;
		}
		if (boolOpNode->getBoolOpCode() == BoolOp::OR && lval == true) {
			// Short-circuit OR operation (if left operand is true, result is true)
			BoolExprAccumulator.push_back(true);  return;
		}
		if (boolOpNode->getBoolOpCode() == BoolOp::NOT) {
			// Perform NOT operation
			BoolExprAccumulator.push_back(!lval); return;
		}
		// Evaluate the right operand
		boolOpNode->getRight()->accept(this);
		bool rval = BoolExprAccumulator.back(); BoolExprAccumulator.pop_back();
		switch (boolOpNode->getBoolOpCode())
		{
		// Perform AND and OR operations 
		case BoolOp::AND:
			BoolExprAccumulator.push_back(lval && rval); return;
		case BoolOp::OR:
			BoolExprAccumulator.push_back(lval || rval); return;
		default:
			// This error should not occur because it should have already been handled in previous stages
			throw SemanticError("INVALID boolean operator");
			return;
		}
	}

private:
	std::vector<long int> NumExprAccumulator;
	std::vector<bool> BoolExprAccumulator;

	SymbolTable& ST;
};
#endif
