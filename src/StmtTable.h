#ifndef STMTTABLE_H
#define STMTTABLE_H

#include "Stmt.h";
#include <vector>

using namespace std;

class StmtTable {
private:
	vector<Stmt> * stmtTable;

public:
	StmtTable();
	~StmtTable();

	//! For Design Extractor's use only.
	void setCurrStmt (int nodeType, PROCNAME caller, PROCNAME callee);
	void setCurrStmt (int nodeType, PROCNAME caller, PROCNAME callee, string RHSExpr);
	//! Get the total number of stmt
	int getSize();

	//! Get node type of statement at stmtNum.
	int getNodeType (int stmtNum);
	//! Get procedure this statement belongs to.
	PROCNAME getCaller (int stmtNum);
	//! If statement is Call, get callee (procedure it calls).
	PROCNAME getCallee (int stmtNum);
	//! If statement is Assign, get RHS expression.
	string getAssignStmtRHSExpr (int stmtNum);
};
#endif