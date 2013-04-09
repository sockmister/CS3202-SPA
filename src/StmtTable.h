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

	void setCurrStmt (int nodeType, PROCNAME caller, PROCNAME callee);

	int getNodeType (int stmtNum);
	PROCNAME getCaller (int stmtNum);
	PROCNAME getCallee (int stmtNum);
};
#endif