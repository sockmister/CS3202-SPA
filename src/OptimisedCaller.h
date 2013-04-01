#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "AST.h"
#include "TNode.h"
#include "ProcTable.h"

using namespace std;


class OptimisedCaller
{
public:
	OptimisedCaller(AST *, ProcTable *);
	void generateOptimised();

	vector<string> getAllStmtLst();
	vector<string> getAllStmt();
	vector<string> getAllAssignStmt();
	vector<string> getAllWhileStmt();
	vector<string> getAllIfStmt();
	vector<string> getAllCallsStmt();
	vector<string> getAllConstant();
private:
	AST * ast;
	ProcTable * procTable;
	vector<string> stmtLstVector;
	vector<string> stmtNumberVector;
	vector<string> assignStmtNumberVector;
	vector<string> whileStmtNumberVector;
	vector<string> ifStmtNumberVector;
	vector<string> callStmtNumberVector;
	vector<string> constantVector;

	void generateStmtLst();
	void generateStmtNum();
	void generateAssignStmtNum();
	void generateWhileStmtNum();
	void generateIfStmtNum();
	void generateCallsStmtNum();
	void generateConstant();

	vector<string> convertToStringResults(vector<int> _intResults);
};
