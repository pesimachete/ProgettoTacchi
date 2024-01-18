#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <fstream>

// Including "token.h" because it's needed for using tokens, and there's no risk of infinite inclusion
// since "tokenizer.h" is not included in "token.h".
#include "token.h"

class tokenizer
{
public: 
	// Overloading the () operator to perform tokenization of the input file.
	// Returns a vector of tokens resulting from the tokenization process.
	std::vector<token> operator()(std::ifstream& inputFile) {
		std::vector<token> inputTokens;
		tokenizeInputFiles(inputFile, inputTokens);
		return inputTokens;
	}

private:
	// The actual tokenization is done by this function, which takes the input file and a token vector as parameters.
	void tokenizeInputFiles(std::ifstream& inputFile, std::vector<token>& inputTokens);
};


#endif // !1



