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

public:
	Stmt();
	Stmt(int nodeTypeFromAST, PROCNAME caller, PROCNAME callee);
	~Stmt();

	void setNodeType (int nodeTypeFromAST);
	void setCaller (PROCNAME callee);
	void setCallee (PROCNAME caller);

	int getNodeType ();
	PROCNAME getCaller ();
	PROCNAME getCallee ();
};
#endif