#include "Exceptions.h"
#include "token.h"
#include "Parser.h"

#include <sstream>
#include <string>
#include <iostream>


Program* Parser::programParse(std::vector<token>::const_iterator& tokenItr)
{
	// A program can only derive into a block, so I need to call blockParse
	Block* mainbb = blockParse(tokenItr);

	// Creating a Program object using the Block parsed
	return PM.makeProgram(mainbb);
}

// Parsing for a block
Block* Parser::blockParse(std::vector<token>::const_iterator& tokenItr)
{

    if (tokenItr->tag == token::LP) {
		// Create the block that will be returned through the Manager
		Block* temp = BM.makeBlock();
		Statement* part;
		Parser::safe_next(tokenItr);
		// If I encounter the BLOCK token, call statementParse until I encounter a ")" which indicates the end of the block
		if (tokenItr->tag == token::BLOCK) {
			safe_next(tokenItr);
			do {
				part = statementParse(tokenItr);
				safe_next(tokenItr);
				// Add the statements to the block
				temp->pushback(part);

			} while (!(tokenItr->tag == token::RP));
		}
		else if (tokenItr->tag == token::IF || tokenItr->tag == token::WHILE || tokenItr->tag == token::PRINT || tokenItr->tag == token::SET || tokenItr->tag == token::INPUT) {
			// In case the block is a single statement, return to "(" since that will be part of statementParse
			--tokenItr;
			ire--;
			part = statementParse(tokenItr);
			// Add the statement to the block 
			temp->pushback(part);
		}
		else {
			std::stringstream tmp{};
			tmp << "ERROR in block definition at word: " << tokenItr->word << " [position: "<< ire << "]";
			throw ParseError(tmp.str());
			return nullptr;
		}
		// Return a pointer to the Block already allocated by BM
		return temp;
	}else
	{
		std::stringstream tmp{};
		tmp << "ERROR: Unexpected initial token for a block at word:  " << tokenItr->word << " [position: " << ire << "]";
		throw ParseError(tmp.str());
		return nullptr;
	}
}
// Parsing for a statement
Statement* Parser::statementParse(std::vector<token>::const_iterator& tokenItr)
{
	if (tokenItr->tag == token::LP) {
		safe_next(tokenItr);
		Statement* temp = nullptr;
		// Parsing different types of statements
		// Each type of statement will call the necessary parsing methods to create its attributes
		// And then allocate a statement using SM
		if (tokenItr->tag == token::IF) {
			safe_next(tokenItr);
			BoolExpr* be = boolexprParse(tokenItr);
			Block* ifblock = blockParse(tokenItr);
			safe_next(tokenItr);
			Block* elseblock = blockParse(tokenItr);
			safe_next(tokenItr);
			temp = SM.makeIfStmt(be, ifblock, elseblock);
		}
		else if (tokenItr->tag == token::INPUT) {
			// Parsing an input statement
			safe_next(tokenItr);
			NumExpr* ni = numexprParse(tokenItr);
			//TODO solve dynamic casting 
			Variable* vi = (Variable*)ni;
			if (!vi) {
				std::stringstream tmp{};
				tmp << "ERROR: Unrecognized variable at word: " << tokenItr->word << " [position: " << ire << "]";
				throw ParseError(tmp.str());
			}
			temp = SM.makeInputStmt(vi);
		}
		else if (tokenItr->tag == token::PRINT) {
			// Parsing a print statement
			safe_next(tokenItr);
			NumExpr* np = numexprParse(tokenItr);
			temp = SM.makePrintStmt(np);
		}
		else if (tokenItr->tag == token::SET) {
			// Parsing a set statement
			safe_next(tokenItr);
			NumExpr* ns = numexprParse(tokenItr);
			NumExpr* as = numexprParse(tokenItr);
			//TODO solve dynamic casting 
			Variable* vs = (Variable*)ns;
			if (!vs) {
				std::stringstream tmp{};
				tmp << "ERROR: Unrecognized variable at word: " << tokenItr->word << " [position: " << ire << "]";
				throw ParseError(tmp.str());
			}
			temp = SM.makeSetStmt(vs, as);
		}
		else if (tokenItr->tag == token::WHILE) {
			// Parsing a while statement
			safe_next(tokenItr);
			BoolExpr* b = boolexprParse(tokenItr);			
			Block* bb = blockParse(tokenItr);
			safe_next(tokenItr);
			temp = SM.makeWhileStmt(b, bb);
		}
		else{
			std::stringstream tmp{};
			tmp << "ERROR: Unrecognized variable at word: " << tokenItr->word << " [position: " << ire << "]";
			throw ParseError(tmp.str());
		}
		if (tokenItr->tag != token::RP) {
			std::stringstream tmp{};
			tmp << "ERROR: Mismatched parenthesis at word: " << tokenItr->word << " [position: " << ire << "]";
			throw ParseError(tmp.str());
		}
		return temp;
	}
	else
	{
		std::stringstream tmp{};
		tmp << "ERROR: Unexpected initial token for a Statement at word:  " << tokenItr->word << " [position: " << ire << "]";
		throw ParseError(tmp.str());
		return nullptr;
	}
}

// Parsing for a numerical expression
NumExpr* Parser::numexprParse(std::vector<token>::const_iterator& tokenItr)
{
	// A correct numerical expression starts with a number, variable_id, or a (
	if (tokenItr->tag == token::LP) {
		safe_next(tokenItr);
		// Identify the type of operation to perform
		Operator::OpCode op;
		switch (tokenItr->tag) {
		case token::ADD: op = Operator::ADD; break;
		case token::SUB: op = Operator::SUB; break;
		case token::MUL: op = Operator::MUL; break;
		case token::DIV: op = Operator::DIV; break;
		default:
			std::stringstream tmp{};
			tmp << "ERROR: Unrecognized operator at word: " << tokenItr->word << " [position: " << ire << "]";
			throw ParseError(tmp.str());
		}
		safe_next(tokenItr);
		// Parse the left and right operands of the operation
		NumExpr* left = numexprParse(tokenItr);
		NumExpr* right = numexprParse(tokenItr);
		if (tokenItr->tag != token::RP) {
			std::stringstream tmp{};
			tmp << "ERROR: Mismatched parenthesis at word: " << tokenItr->word << " [position: " << ire << "]";
			throw ParseError(tmp.str());
		}
		safe_next(tokenItr);
		// Create the operation using the pointers and NEM for allocation
		return NEM.makeOperator(op, left, right);

	}
	else if (tokenItr->tag == token::NUMBER) {
		// Get the value from the token's word
		// Convert it to a long int
		// Create a node using NEM, using the extracted token value
		std::stringstream temp{};
		temp << tokenItr->word;
		long int value;
		temp >> value;
		NumExpr* expr = NEM.makeNumber(value);
		safe_next(tokenItr);
		return expr;
	}
	else if (tokenItr->tag == token::VARIABLE_ID) {
		// Get the value from the token's word
		// Convert it to a string
		// Create a node using NEM, using the extracted token text
		std::stringstream temp{};
		temp << tokenItr->word;
		std::string name;
		temp >> name;
		NumExpr* expr = NEM.makeVariable(name);
		safe_next(tokenItr);
		return expr;
	}
	else {
		std::stringstream tmp{};
		tmp << "ERROR: Unexpected initial token for a Numeric Expression at word:   " << tokenItr->word << " [position: " << ire << "]";
		throw ParseError(tmp.str());
		return nullptr;
	}
}

// Parsing for a boolean expression
BoolExpr* Parser::boolexprParse(std::vector<token>::const_iterator& tokenItr)
{	
	// A correct boolean expression starts with a ( or a boolean constant
	if (tokenItr->tag == token::LP) {
		safe_next(tokenItr);
		BoolExpr* temp;
		// Each type of boolean expression will call the necessary parsing methods to create its attributes
		// Then allocate a boolean expression using BEM
		if (tokenItr->tag == token::LT || tokenItr->tag == token::GT || tokenItr->tag == token::EQ)
		{
			RelOp::RelOpCode cnum = RelOp::tokenTorelopcode(tokenItr->tag);
			safe_next(tokenItr);
			NumExpr* leftnum = numexprParse(tokenItr);
			NumExpr* rightnum = numexprParse(tokenItr);
			temp = BEM.makeRelOp(cnum, leftnum, rightnum);
		}
		else if (tokenItr->tag == token::AND || tokenItr->tag == token::OR) {
			BoolOp::BoolOpCode cbool = BoolOp::tokenToboolopcode(tokenItr->tag);
			safe_next(tokenItr);
			BoolExpr* leftbool = boolexprParse(tokenItr);
			BoolExpr* rightbool = boolexprParse(tokenItr);
			temp = BEM.makeBoolOp(cbool, leftbool, rightbool);
		}
		else if (tokenItr->tag == token::NOT) {
			BoolOp::BoolOpCode c = BoolOp::NOT;
			safe_next(tokenItr);
			BoolExpr* notbool = boolexprParse(tokenItr);
			temp = BEM.makeBoolOp(c, notbool);
		}
		else{
			std::stringstream tmp{};
			tmp << "ERROR: Unrecognized operator in boolean expression at word: " << tokenItr->word << " [position: " << ire << "]";
			throw ParseError(tmp.str());
			return nullptr;
		}

		if (tokenItr->tag != token::RP) {
			std::stringstream tmp{};
			tmp << "ERROR: Mismatched parenthesis at word: " << tokenItr->word << " [position: " << ire << "]";
			throw ParseError(tmp.str());
			return nullptr;
		}
		safe_next(tokenItr);
		return temp;
	}
	else if (tokenItr->tag == token::TRUE || tokenItr->tag == token::FALSE) {
		BoolExpr* bc = BEM.makeBoolConst(BoolConst::tokenTobool(tokenItr->tag));
		safe_next(tokenItr);
		return bc;
	}
}

