#ifndef TOKEN_H
#define TOKEN_H

#include <string>


// The tokens in this program are those described by the context-free grammar.

// Even though NUMBER or VARIABLE_ID are not terminal symbols, they can be defined by regular expressions.
// Therefore, there's no need to interpret them through the parser.
// Lexical analysis can already understand which tokens will be numbers or variable names in situations where there's something that would be of type Number or Variable_id.

class token
{
	public:
	static constexpr int BLOCK = 0;
	static constexpr int INPUT = 1;
	static constexpr int PRINT = 2;
	static constexpr int SET = 3;
	static constexpr int WHILE = 4;
	static constexpr int IF = 5;
	static constexpr int GT = 6;
	static constexpr int LT = 7;
	static constexpr int EQ = 8;
	static constexpr int AND = 9;
	static constexpr int OR = 10;
	static constexpr int NOT = 11;
	static constexpr int ADD = 12;
	static constexpr int SUB = 13;
	static constexpr int MUL = 14;
	static constexpr int DIV = 15;
	static constexpr int TRUE = 16;
	static constexpr int FALSE = 17;
	static constexpr int LP = 18;
	static constexpr int RP = 19;
	static constexpr int NUMBER = 20;
	static constexpr int VARIABLE_ID = 21;


	static constexpr const char* id2word[]{
		"BLOCK", "INPUT", "PRINT", "SET", "WHILE", "IF", "GT", "LT", "EQ", "AND", "OR", "NOT", "ADD", "SUB", "MUL", "DIV", "TRUE", "FALSE", "(", ")", "NUMBER", "VARIABLE_ID"
	};

	// By creating constructors with parameters, the default constructor without parameters is automatically deleted.

	token(int t, const char* w) : tag{ t }, word{ w } { }

	token(int t, std::string w) : tag{ t }, word{ w } { }

	

	int tag;
	std::string word;

};

// I've overloaded the insertion operator to perform tests on the tokenizer.
std::ostream& operator<<(std::ostream& os, const token& t);


#endif // !TOKEN_H



