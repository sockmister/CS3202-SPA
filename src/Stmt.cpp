#include "Stmt.h"

Stmt::Stmt()
{
}

Stmt::Stmt(int nodeType, PROCNAME caller, PROCNAME callee)
{
	this->setNodeType(nodeType);
	this->setCaller(caller);
	this->setCallee(callee);
}

Stmt::~Stmt()
{
}

void Stmt::setNodeType(int nodeType)
{
	this->nodeType = nodeType;
}

void Stmt::setCaller(PROCNAME caller)
{
	this->caller = caller;
}

void Stmt::setCallee(PROCNAME callee)
{
	this->callee = callee;
}

int Stmt::getNodeType()
{
	return this->nodeType;
}

PROCNAME Stmt::getCaller()
{
	return this->caller = caller;
}

PROCNAME Stmt::getCallee()
{
	return this->callee = callee;
}