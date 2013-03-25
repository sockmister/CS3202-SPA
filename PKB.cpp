#include "PKB.h"

PKB::PKB(){
}

PKB::PKB(AST * ast, VarTable * varTable, Follows * follows, Parent * parent, Modifies * modifies, Uses * uses, ProcTable * procTable, Calls * calls):
	ast(ast),
	varTable(varTable),
	follows(follows),
	parent(parent),
	modifies(modifies),
	uses(uses),
	procTable(procTable),
	calls(calls)
	
{
}

PKB::~PKB(){
	delete ast;
	delete varTable;
	delete follows;
	delete parent;
	delete modifies;
	delete uses;
	delete calls;
	delete procTable;
}

void PKB::setAST(AST * ast){
	this->ast = ast;
}

void PKB::setVarTable(VarTable * varTable){
	this->varTable = varTable;
}

void PKB::setFollows(Follows * follows){
	this->follows = follows;
}

void PKB::setParent(Parent * parent){
	this->parent = parent;
}

void PKB::setModifies(Modifies * modifies){
	this->modifies = modifies;
}

void PKB::setUses(Uses * uses){
	this->uses = uses;
}

void PKB::setCalls(Calls * calls){
	this->calls = calls;
}

void PKB::setProcTable(ProcTable * procTable){
	this->procTable = procTable;
}

AST* PKB::getAST(){
	return this->ast;
}

VarTable* PKB::getVarTable(){
	return this->varTable;
}

Follows* PKB::getFollows(){
	return this->follows;
}

Parent* PKB::getParent(){
	return this->parent;
}

Modifies * PKB::getModifies(){
	return this->modifies;
}

Uses * PKB::getUses(){
	return this->uses;
}

Calls * PKB::getCalls(){
	return this->calls;
}

ProcTable * PKB::getProcTable(){
	return this->procTable;
}