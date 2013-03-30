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
	//! Default constructor
	Affects(AST * ast, Modifies  * mod, Uses  * use, VarTable * var, ProcTable * proc);
	
	//! Destructor
	~Affects(void);

	//! Returns whether affects(a1, a2) is true
	bool isAffects(STMT a1, STMT a2);

	//! Returns whether if affects*(a1, a2) is true
	bool isAffectsStar(STMT a1, STMT a2);

private:
	bool isNotModifiedInAControlFlow(STMT a1, STMT a2, VARNAME v);
	// Returns all traversed paths after and not including a1 up to and including a2
	vector<STMTLST> findAllPaths(STMT a1, STMT a2, STMTLST path, bool firstPass);
	string getProcedureName(STMT s);
	string getSameProcedure(STMT a1, STMT a2);
	bool isAffectsStarRecurse(STMT a1, STMT a2);
	
	/* PKB used during computing of Affects */
	AST * ast;
	Modifies * modifies;
	Uses * uses;
	VarTable * varTable;
	ProcTable * procTable;
	CFG * cfg;
};
