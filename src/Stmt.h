#ifndef STMT_H
#define STMT_H

#include <string>

using namespace std;

typedef string PROCNAME; //Procedure name

class Stmt {
private:
	int nodeType;
	PROCNAME caller;
	PROCNAME callee;
	string assignStmtRHSExpr;

public:
	Stmt();
	Stmt(int nodeTypeFromAST, PROCNAME caller, PROCNAME callee);
	Stmt(int nodeTypeFromAST, PROCNAME caller, PROCNAME callee, string RHSexpr);
	~Stmt();

	void setNodeType (int nodeTypeFromAST);
	void setCaller (PROCNAME callee);
	void setCallee (PROCNAME caller);
	void setAssignStmtRHSExpr (string RHSexpr);

	int getNodeType ();
	PROCNAME getCaller ();
	PROCNAME getCallee ();
	string getAssignStmtRHSExpr ();
};
#endif