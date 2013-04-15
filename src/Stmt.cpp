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

Stmt::Stmt(int nodeType, PROCNAME caller, PROCNAME callee, string RHSexpr)
{
	this->setNodeType(nodeType);
	this->setCaller(caller);
	this->setCallee(callee);
	this->setAssignStmtRHSExpr(RHSexpr);
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

void Stmt::setAssignStmtRHSExpr(string RHSExpr)
{
	this->assignStmtRHSExpr = RHSExpr;
}

int Stmt::getNodeType()
{
	return this->nodeType;
}

PROCNAME Stmt::getCaller()
{
	return this->caller;
}

PROCNAME Stmt::getCallee()
{
	return this->callee;
}

string Stmt::getAssignStmtRHSExpr()
{
	return this->assignStmtRHSExpr;
}