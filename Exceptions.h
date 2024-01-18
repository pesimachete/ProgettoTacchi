#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

// Extending the "std::runtime_error" class in four different contexts to help pinpoint the stage of the program where the issue occurred.

struct LexicalError : std::runtime_error {
	LexicalError(const char* msg) : std::runtime_error(msg) { }
	LexicalError(std::string msg) : std::runtime_error(msg.c_str()) { }
};

struct ParseError : std::runtime_error {
	ParseError(const char* msg) : std::runtime_error(msg) { }
	ParseError(std::string msg) : std::runtime_error(msg.c_str()) { }
};
struct ConversionError : std::runtime_error {
	ConversionError(const char* msg) : std::runtime_error(msg) { }
	ConversionError(std::string msg) : std::runtime_error(msg.c_str()) { }
};
struct SemanticError : std::runtime_error{
	SemanticError(const char* msg) : std::runtime_error(msg) { }
	SemanticError(std::string msg) : std::runtime_error(msg.c_str()) { }
};

#endif