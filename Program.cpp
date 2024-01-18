#include "Program.h"
#include "Visitor.h"

// Allowing it to accept the visitor by invoking the visit to the program.

void Program::accept(Visitor* v)
{
	v->visitProgram(this);
}
