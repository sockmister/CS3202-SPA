#include "StmtTable.h"

StmtTable::StmtTable()
{
	this->stmtTable = new vector<Stmt>();
	this->stmtTable->push_back(Stmt()); //push dummy element to fill index 0
}

StmtTable::~StmtTable()
{
	delete this->stmtTable;
}

//! For Design Extractor's use only.
void StmtTable::setCurrStmt(int nodeType, PROCNAME caller, PROCNAME callee)
{
	Stmt currStmt = Stmt(nodeType, caller, callee);
	this->stmtTable->push_back(currStmt);
}

//! Get node type of statement at stmtNum.
int StmtTable::getNodeType (int stmtNum)
{
	return this->stmtTable->at(stmtNum).getNodeType();
}

//! Get procedure this statement belongs to.
PROCNAME StmtTable::getCaller (int stmtNum)
{
	return this->stmtTable->at(stmtNum).getCaller();
}

//! If statement is Call, get callee (procedure it calls).
PROCNAME StmtTable::getCallee (int stmtNum)
{
	return this->stmtTable->at(stmtNum).getCallee();
}