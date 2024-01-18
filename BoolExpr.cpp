#include "BoolExpr.h"
#include "Visitor.h"

// To accept the visitor, invoke the corresponding function of the Visitor class

void RelOp::accept(Visitor* v)
{
	v->visitRelOp(this);
}

void BoolConst::accept(Visitor* v)
{
	v->visitBoolConst(this);
}

void BoolOp::accept(Visitor* v)
{
	v->visitBoolOp(this);
}
