#include <sstream>


#include "token.h"


std::ostream& operator<<(std::ostream& os, const token& t) {
	std::stringstream tmp; 
	tmp << t.tag << ": " << t.word; 
	os << tmp.str();
	return os; 
}
