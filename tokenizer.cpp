#include <string>
#include <sstream>
#include <iostream>

#include "tokenizer.h"
#include "Exceptions.h"


// The purpose of the function definition is to process character by character and compare them with terminal symbols.
// When the string consists only of uppercase or lowercase letters and there's no match with other tokens,
// a variable is created with the name from the current string.

void tokenizer::tokenizeInputFiles(std::ifstream& inputFile, std::vector<token>& inputTokens) {
	std::string parole;
	if (inputFile.eof()) {
		throw LexicalError("Empty program");
		return;
	}
	char ch = inputFile.get();
	
	while (std::isspace(ch)) {
		ch = inputFile.get();
	}
	parole.push_back(ch);
	// Continue the loop until the characters from the file are exhausted
	while (!inputFile.eof()) {
		if (ch == '(') {
			// If the last character was an open parenthesis, it's possible that there was something like "variable_id(".
			// (It's not possible for anything else to be there, as it would have been caught earlier when the parenthesis wasn't present yet.)
			// So, knowing that there was a parenthesis and that the variable name would end there,
			// remove the parenthesis from the string and check if all other characters are letters (criteria for VARIABLE_ID).
			// Add the VARIABLE_ID token to the vector before the "(".
			parole.pop_back();
			if (!parole.empty()) {
				for (char cr : parole) {
					if (!isalpha(cr)) {
						std::stringstream tmp{};
						tmp << "Invalid variable name: " << parole;
						throw LexicalError(tmp.str());
					}
				}
				inputTokens.push_back(token{ token::VARIABLE_ID, parole.c_str() });
			}
			inputTokens.push_back(token{ token::LP, token::id2word[token::LP] });
			parole.clear();
			ch = inputFile.get();
			while (std::isspace(ch)) {
				ch = inputFile.get();
			}
			parole.push_back(ch);
			continue;
			
		}
		else if (ch == ')')
		{
			// Following the same logic as with open parenthesis, check if there was a variable name before the ")"
			parole.pop_back();
			if (!parole.empty()) {
				for (char cr : parole) {
					if (!isalpha(cr)) {
						std::stringstream tmp{};
						tmp << "Lexical error on word: " << parole;
						throw LexicalError(tmp.str());
					}
				}
				inputTokens.push_back(token{ token::VARIABLE_ID, parole.c_str() });
			}
			inputTokens.push_back(token{ token::RP, token::id2word[token::RP] });
			parole.clear();
			ch = inputFile.get();
			while (std::isspace(ch)) {
				ch = inputFile.get();
			}
			parole.push_back(ch);
			continue;
		}
		// From here, check if the word in "parole" matches any of the tokens.
		else if (!parole.compare("BLOCK"))
		{
			inputTokens.push_back(token{ token::BLOCK, token::id2word[token::BLOCK] });
			parole.clear();
		}
		else if (!parole.compare("INPUT"))
		{
			inputTokens.push_back(token{ token::INPUT, token::id2word[token::INPUT] });
			parole.clear();
		}
		else if (!parole.compare("PRINT"))
		{
			inputTokens.push_back(token{ token::PRINT, token::id2word[token::PRINT] });
			parole.clear();
		}
		else if (!parole.compare("SET"))
		{
			inputTokens.push_back(token{ token::SET, token::id2word[token::SET] });
			parole.clear();
		}
		else if (!parole.compare("WHILE"))
		{
			inputTokens.push_back(token{ token::WHILE, token::id2word[token::WHILE] });
			parole.clear();
		}
		else if (!parole.compare("IF"))
		{
			inputTokens.push_back(token{ token::IF, token::id2word[token::IF] });
			parole.clear();
		}
		else if (!parole.compare("GT"))
		{
			inputTokens.push_back(token{ token::GT, token::id2word[token::GT] });
			parole.clear();
		}
		else if (!parole.compare("LT"))
		{
			inputTokens.push_back(token{ token::LT, token::id2word[token::LT] });
			parole.clear();
		}
		else if (!parole.compare("EQ"))
		{
			inputTokens.push_back(token{ token::EQ, token::id2word[token::EQ] });
			parole.clear();
		}
		else if (!parole.compare("AND"))
		{
			inputTokens.push_back(token{ token::AND, token::id2word[token::AND] });
			parole.clear();
		}
		else if (!parole.compare("OR"))
		{
			inputTokens.push_back(token{ token::OR, token::id2word[token::OR] });
			parole.clear();
		}
		else if (!parole.compare("NOT"))
		{
			inputTokens.push_back(token{ token::NOT, token::id2word[token::NOT] });
			parole.clear();
		}
		else if (!parole.compare("ADD"))
		{
			inputTokens.push_back(token{ token::ADD, token::id2word[token::ADD] });
			parole.clear();
		}
		else if (!parole.compare("SUB"))
		{
			inputTokens.push_back(token{ token::SUB, token::id2word[token::SUB] });
			parole.clear();
		}
		else if (!parole.compare("MUL"))
		{
			inputTokens.push_back(token{ token::MUL, token::id2word[token::MUL] });
			parole.clear();
		}
		else if (!parole.compare("DIV"))
		{
			inputTokens.push_back(token{ token::DIV, token::id2word[token::DIV] });
			parole.clear();
		}
		else if (!parole.compare("TRUE"))
		{
			inputTokens.push_back(token{ token::TRUE, token::id2word[token::TRUE] });
			parole.clear();
		}
		else if (!parole.compare("FALSE"))
		{
			inputTokens.push_back(token{ token::FALSE, token::id2word[token::FALSE] });
			parole.clear();
		} 
		// If the first character of "parole" is a digit or minus sign, it indicates a number.
		// Extract the entire numeric string, check its validity, and add the NUMBER token to the vector.
		else if (parole.c_str()[0] == '-' || std::isdigit(parole.c_str()[0])) 
		{
			// If we're here, "word" is a single character (ch), which is either a minus sign or a number.
			// Add ch to "tmp," which will be the string containing the entire number.
			std::string tmp{};
			tmp.push_back(ch);

			// Loop to collect all digits and add them to "tmp" 
			do {
				ch = inputFile.get();
				if (std::isdigit(ch)) tmp.push_back(ch);
			} while (std::isdigit(ch));

			// Check if the number starts with '0' and continues, as this is not allowed by the grammar.
			if (tmp.c_str()[0] == '0' && tmp.length() > 1) {
				std::stringstream tmp1{};
				tmp1 << "Invalid number: " << tmp;
				throw LexicalError(tmp1.str());
			}
			inputTokens.push_back(token{ token::NUMBER, tmp });
			parole.clear();

			// Check if there's a space or parenthesis after the number.
			// If not, it means there are two adjacent tokens that can't be.
			if (std::isspace(ch)) {

			}
			else if (ch == '(' || ch == ')') {
				parole.push_back(ch);
			}
			else if (inputFile.eof()) {

			}
			else {
				std::stringstream tmp{};
				tmp << "Lexical error on word: " << ch;
				throw LexicalError(tmp.str());
			}
			continue;
		}
		else {
			// If the word doesn't match any token, process the next character.
			// If it's a space, it indicates the previous word ended, and it is a VARIABLE_ID.
			// If not, it means the previous word isn't finished yet, so add the character to "parole"
			ch = inputFile.get();
			if (std::isspace(ch)) {
				for (char cr : parole) {
					if (!isalpha(cr)) {
						std::stringstream tmp{};
						tmp << "Lexical error on word: " << parole;
						throw LexicalError(tmp.str());
					}
				}
				inputTokens.push_back(token{ token::VARIABLE_ID, parole });
				parole.clear();
				while (std::isspace(ch)) {
					ch = inputFile.get();
				}
				parole.push_back(ch);

			}
			else
			{
				parole.push_back(ch);
			}
			continue;
		}

		// If execution reaches here, it means there was a match with a token that wasn't '(' or ')', NUMBER, or VARIABLE_ID and "word" is empty.
		// Since a token was just processed, the next character should be a space or parenthesis.
		// If it's a space, skip all spaces.
		ch = inputFile.get();
		if (std::isspace(ch)) {
			while (std::isspace(ch)) {
				ch = inputFile.get();
			}
		}
		else if (ch == '(' || ch == ')') {

		}
		else if(inputFile.eof()) {

		}
		else{
			std::stringstream tmp{};
			tmp << "Lexical error on character: " << ch;
			throw LexicalError(tmp.str());
		}
		parole.push_back(ch);
	}
}


