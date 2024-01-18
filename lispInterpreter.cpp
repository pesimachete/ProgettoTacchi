
#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <string>

#include "Exceptions.h"
#include "token.h"
#include "tokenizer.h"
#include "Manager.h"
#include "Parser.h"
#include "Visitor.h"
#include "SymbolTable.h"

int main(int argc, char* argv[])
{
    // Retrieve the filename from the program's arguments
    // In case of missing arguments, the program exits with an error
    if (argc < 2) {
        std::cerr << "Not specified file!" << std::endl;
        std::cerr << "Use: " << argv[0] << " <nome_file>" << std::endl;
        return EXIT_FAILURE;
    }
    // Try to open the file specified in the passed argument and handle exceptions in case of opening error 
    std::ifstream inputFile;
    try {
        inputFile.open(argv[1]);
    }
    catch (std::exception& exc) {
        std::cerr << "Cannot open " << argv[1] << std::endl;
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }
    // Instantiate the tokenizer, which as a Function Class will be responsible for populating the inputTokens vector with tokens encountered in the opened file
    tokenizer tokenize;
    std::vector<token> inputTokens;

    try {
        // Call the () function on inputFile and use std::move to transfer the returned vector from the function 
        inputTokens = std::move(tokenize(inputFile));
        inputFile.close(); // Close the file since the information is now in the inputTokens vector
    }
    catch (LexicalError& le) {
        // Catch exceptions propagated from lexical error-related issues 
        std::cerr << "Lexical Error" << std::endl;
        std::cerr << le.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception& exc) {
        // Catch exceptions propagated from any other sources that might throw an exception
        std::cerr << "Cannot read from  " << argv[1] << std::endl;
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Instantiate all the managers necessary for allocating nodes in the syntax tree to be created during the semantic analysis phase
    BlockManager BM;
    BoolExprManager BEM;
    NumExprManager NEM;
    StatementManager SM;
    ProgramManager PM;
    
    // Instantiate a SymbolTable to manage variable allocation and values during program interpretation
    SymbolTable ST;

    // Lastly, instantiate the Function Class responsible for parsing
    Parser parse{ NEM,BEM,SM,BM,PM };

    try {
        // Call the () function on inputTokens, returning a pointer to the Program node, which is the initial node of the syntax tree
        Program* p = parse(inputTokens);

        // Uncomment the following lines to enable printing the syntax tree
        // PrintVisitor* vipi = new PrintVisitor();
        // p->accept(vipi);

        // Instantiate a visitor responsible for evaluating the syntax tree
        EvaluatorVisitor* viev = new EvaluatorVisitor(ST);
        // When p accepts the program, the visitor starts traversing the tree and interpreting the program
        p->accept(viev);
    }
    catch (ParseError& pe) {
        // Catch exceptions propagated from parsing errors
        std::cerr << "Error in parsing" << std::endl;
        std::cerr << pe.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (SemanticError& se) {
        // Catch exceptions propagated from interpretation and semantic errors in the program
        std::cerr << "Error in semantic analysis" << std::endl;
        std::cerr << se.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception& exc) {
        // Catch exceptions propagated from any other sources that might throw an exception
        std::cerr << "Error" << std::endl;
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;


    // cosa devo ancora fare : 
    // dynamic cast in parsing Set 
    // fare uml 
}