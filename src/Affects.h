#pragma once
#include <string>
#include <iostream>
#include <vector>

#include "AST.h"
#include "Uses.h"
#include "Modifies.h"
#include "VarTable.h"
#include "ProcTable.h"

using namespace std;
typedef int STMT;
typedef vector<STMT> STMTLST;

class Affects
{
public:
	Affects(AST * ast, Modifies  * mod, Uses  * use, VarTable * var, ProcTable * proc);
	~Affects(void);
	bool isAffects(STMT a1, STMT a2);
	bool isAffectsStar(STMT a1, STMT a2);
	STMTLST affectsStatement (int order, STMT a);
	STMTLST affectsStatementStar (int order, STMT a);
	string getProcedureName(STMT s);
	bool isModifiedInCalls(STMT s);
	bool isNotModifiedInAControlFlow(STMT a1, STMT a2, VARNAME v);
	vector<STMTLST> findAllPaths(STMT a1, STMT a2, STMTLST path);

private:
	bool isSameProcedure(STMT a1, STMT a2);
	STMTLST getSequentialControlFlow(STMT a1, STMT a2, STMTLST sortedControlFlow);

	AST * ast;
	Modifies * modifies;
	Uses * uses;
	VarTable * varTable;
	ProcTable * procTable;
	CFG * cfg;
};
