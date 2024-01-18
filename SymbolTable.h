#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include<string>
#include<vector>
#include "Exceptions.h"

// Represents a symbol in the symbol table.
struct Symbol
{
	Symbol(std::string vi, long int vu) :var_id{ vi }, value{ vu } {}

	std::string var_id; // Variable identifier
	long int value;			// Value associated with the variable
};

class SymbolTable
{
public: 

	SymbolTable() = default;

	SymbolTable& operator=(const SymbolTable& st) = delete;
	SymbolTable(const SymbolTable& st) = delete;

	~SymbolTable() {

		// Destructor: Frees memory occupied by stored symbols
		auto i = variables.begin();
		for (; i != variables.end(); ++i) {
			delete(*i);
		}
		variables.resize(0);
	}
	
	// Create or update a variable in the symbol table
	void CCvar(std::string vi, long int vu) {
		for (auto i : variables) {
			if (i->var_id == vi) {
				i->value = vu; // Update the value if variable exists
				return;
			}
		}
		// Create a new Symbol and add the pointer to the vector 
		Symbol* created = new Symbol(vi, vu);
		variables.push_back(created);
	}

	// Retrieve the value of a variable from the symbol table
	long int getValueFromVariable(std::string vi) const {
		for (auto i : variables) {
			if (i->var_id == vi) {
				return i->value; // Return the value if variable exists
			}
		}
		throw SemanticError("Variable does not exist"); // Variable not found
	}

private: 
	std::vector<Symbol*> variables;
	
};

#endif