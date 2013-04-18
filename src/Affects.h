#pragma once
#include <string>
#include <iostream>
#include <vector>

#include "AST.h"
#include "Uses.h"
#include "Modifies.h"
#include "VarTable.h"
#include "ProcTable.h"
#include "StmtTable.h"
//#include "AbstractWrapper.h"

/*!  \class Affects.
	 \brief Affects is used to compute Affects relation condition.
*/

using namespace std;
typedef int STMT;
typedef vector<STMT> STMTLST;

class Affects
{
public:
	//! Default constructor
	Affects(AST * ast, Modifies  * mod, Uses  * use, VarTable * var, ProcTable * proc, rootWhile * rootwhile);
	
	//! Destructor
	~Affects(void);

	//! Returns whether affects(a1, a2) is true
	bool isAffects(STMT a1, STMT a2);

	//! Returns whether if affects*(a1, a2) is true
	bool isAffectsStar(STMT a1, STMT a2);
	
	//! Returns whether affectsBip(a1, a2) is true
	//bool isAffectsBip(STMT a1, STMT a2);

	////! Returns whether if affectsBip*(a1, a2) is true
	//bool isAffectsBipStar(STMT a1, STMT a2);

	//! Creates an n by n table where n is the total number of statements. Used in Design Extractor
	void initializeCache(void);
	void initializeStmtTable(StmtTable * st);

	// Seperate for testing
	void testFindAllPaths(STMT a1, STMT a2);

private:
	// For Affects*
	STMTLST getStmtLstAffectingStmt(STMT s, STMTLST range);
	VARLIST getVariablesUsedinStmt(STMT s);
	STMTLST getRange(STMT a1, STMT a2);
	bool isAffectsStarRecurseRedesigned(STMT a1, STMT a2);
	vector<STMTLST> getAffectsStarRedesignedPath(STMT a1, STMT a2, STMTLST range, STMTLST path, bool firstPass);

	vector<STMTLST> getAffectsStarPath(STMT original_a1, STMT a1, STMT a2, STMTLST path, bool firstPass);
	bool isAffectsStarRecurseOptimised(STMT a1, STMT a2);
	bool isAffectsStarRecurse(STMT a1, STMT a2);
	bool isAffectsStarCompute(STMT a1, STMT a2);
	int getFromAffectsStarCache(STMT query_a1, STMT query_a2, bool scan);
	
	// For Affects	
	vector<STMTLST> getUnmodifiedPath(STMT a1, STMT a2, VARNAME v, STMTLST path, bool firstPass);
	bool isNotModifiedInAControlFlowOptimised(STMT a1, STMT a2, VARNAME v);
	bool isNotModifiedInAControlFlow(STMT a1, STMT a2, VARNAME v);
	bool isAffectsNoScan(STMT a1, STMT a2);
	bool isAffectsCompute(STMT a1, STMT a2);
	int getFromAffectsCache(STMT query_a1, STMT query_a2, bool scan);

	// General
	vector<STMTLST> findAllPaths(STMT a1, STMT a2, STMTLST path, bool firstPass);
	string getProcedureName(STMT s);
	string getSameProcedure(STMT a1, STMT a2);
	
	// PKB used during computing of Affects/Affects*
	AST * ast;
	Modifies * modifies;
	Uses * uses;
	VarTable * varTable;
	ProcTable * procTable;
	rootWhile * rootwhile;
	StmtTable * stmttable;
	CFG * cfg;
	vector<vector<int>> affectsCache;
	vector<vector<int>> affectsStarCache;
	int totalNoOfStatements;
};
