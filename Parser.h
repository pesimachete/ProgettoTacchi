#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include "Exceptions.h"
#include "token.h"
#include "Manager.h"
#include "Program.h"
#include "Block.h"
#include "BoolExpr.h"
#include "NumExpr.h"
#include "Statement.h"

class Parser
{
public:
	// Constructor: Initializes the parser with various managers for managing objects.
	Parser(NumExprManager& n, BoolExprManager& be, StatementManager& sm, BlockManager& bm, ProgramManager& pm) : NEM{ n }, BEM{ be }, SM{ sm }, BM{ bm }, PM{ pm } {}

	// Operator() to start parsing from the given input token stream.
	Program* operator()(const std::vector<token>& inputStream) 
	{
		auto tokenItr = inputStream.begin();
		// Initialize the total length and the current position
		leght = inputStream.size();
		if(leght == 0) throw ParseError("Empty Program");
		ire = 1;
		// Call the parser for the program, as "Program" is the starting symbol of derivation
		Program* pro = programParse(tokenItr);
		// If the parsing function above returns but hasn't reached the end of input, raise an error
		if (ire != leght) {
			throw ParseError("Unexpected premature ending");
		}
		return pro;
	}
private:
	// These managers are responsible for the allocation of all tree nodes that will be created.
	NumExprManager& NEM;
	BoolExprManager& BEM;
	StatementManager& SM;
	BlockManager& BM;
	ProgramManager& PM;

	int leght; // Total length of input token stream.
	int ire;   // Current position in the token stream.

	// Parsers will be called recursively to create the syntactic tree of the program being parsed.
	Program* programParse(std::vector<token>::const_iterator& tokenItr);
	Block* blockParse(std::vector<token>::const_iterator& tokenItr);
	Statement* statementParse(std::vector<token>::const_iterator& tokenItr);
	NumExpr* numexprParse(std::vector<token>::const_iterator& tokenItr);
	BoolExpr* boolexprParse(std::vector<token>::const_iterator& tokenItr);


	// Helper function to safely move to the next token.
	void safe_next(std::vector<token>::const_iterator& itr) {
		if (ire  < leght) {
			itr++;
			ire++;
			return;
		}
		throw ParseError("Unexpected end of input");
		return;
	}
};

#endif