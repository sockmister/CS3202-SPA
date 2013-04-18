#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <set>

#include "AST.h"
#include "TNode.h"
#include "ProcTable.h"
#include "Follows.h"
#include "Parent.h"
#include "Modifies.h"
#include "Uses.h"
#include "Calls.h"
#include "CFG.h"
#include "rootIf.h"

using namespace std;


class OptimisedCaller
{
public:
	OptimisedCaller(AST *, ProcTable *, Calls *, Follows *, Modifies *, Parent *, Uses *);
	void generateOptimised();
	
	//! Get all stmtLst number
	/*!
		\return a list of statement number for all stmtLst
	*/
	vector<string> getAllStmtLst();

	//! Get all stmt number
	/*!
		\return a list of statement number for all stmt
	*/
	vector<string> getAllStmt();

	//! Get all assign stmt number
	/*!
		\return a list of statement number for all asign stmt
	*/
	vector<string> getAllAssignStmt();

	//! Get all while stmt number
	/*!
		\return a list of statement number for all while stmt
	*/
	vector<string> getAllWhileStmt();

	//! Get all if stmt number
	/*!
		\return a list of statement number for all if stmt
	*/
	vector<string> getAllIfStmt();

	//! Get all call stmt number
	/*!
		\return a list of statement number for all call stmt
	*/
	vector<string> getAllCallsStmt();

	//! Get all constant 
	/*!
		\return a list of integers for all constant
	*/
	vector<string> getAllConstant();

	//! Get all stmtLst number under the entered stmtLst number
	/*!
		\parm int stmtLstNum
		\return a list of stmtLst number under the entered stmtLst number
	*/
	vector<string> getStmtLstStmt(int stmtLstNum);

	//! Get all ifstmtLst number under the entered stmtLst number
	/*!
		\parm int stmtLstNum
		\return a list of ifstmtLst number under the entered stmtLst number
	*/
	vector<string> getIfStmtLstStmt(int stmtLstNum);

	vector<string> getFollows(int, int, string);
	vector<string> getFollowsStar(int, int, string);

	vector<string> getNextStar(int, int, string);

	void initialiseAll();
private:
	AST * ast;
	ProcTable * procTable;
	Calls * calls;
	Follows * follows;
	Modifies * modifies;
	Parent * parent; 
	Uses * uses;

	vector<string> stmtLstVector;
	vector<string> stmtNumberVector;
	vector<string> assignStmtNumberVector;
	vector<string> whileStmtNumberVector;
	vector<string> ifStmtNumberVector;
	vector<string> callStmtNumberVector;
	vector<string> constantVector;
	vector<vector<string>> stmtLstTable;
	vector<vector<string>> ifStmtLstTable;
	vector<string> procedureAllConstantIndex;

	//follows tables
	vector<vector<string>> follows_Left_Assign;
	vector<vector<string>> follows_Left_Calls;
	vector<vector<string>> follows_Left_While;
	vector<vector<string>> follows_Left_If;
	vector<vector<string>> follows_Left_Stmt;

	vector<vector<string>> follows_Right_Assign;
	vector<vector<string>> follows_Right_Calls;
	vector<vector<string>> follows_Right_While;
	vector<vector<string>> follows_Right_If;
	vector<vector<string>> follows_Right_Stmt;
	
	//followsStar tables
	vector<vector<string>> followsStar_Left_Assign;
	vector<vector<string>> followsStar_Left_Calls;
	vector<vector<string>> followsStar_Left_While;
	vector<vector<string>> followsStar_Left_If;
	vector<vector<string>> followsStar_Left_Stmt;

	vector<vector<string>> followsStar_Right_Assign;
	vector<vector<string>> followsStar_Right_Calls;
	vector<vector<string>> followsStar_Right_While;
	vector<vector<string>> followsStar_Right_If;
	vector<vector<string>> followsStar_Right_Stmt;

	//nextStar tables
	vector<vector<string>> nextStar_Left_Assign;
	vector<vector<string>> nextStar_Left_Calls;
	vector<vector<string>> nextStar_Left_While;
	vector<vector<string>> nextStar_Left_If;
	vector<vector<string>> nextStar_Left_Stmt;

	vector<vector<string>> nextStar_Right_Assign;
	vector<vector<string>> nextStar_Right_Calls;
	vector<vector<string>> nextStar_Right_While;
	vector<vector<string>> nextStar_Right_If;
	vector<vector<string>> nextStar_Right_Stmt;
	
	///////
	//Methods
	void generateStmtLst();
	void generateStmtNum();
	void generateAssignStmtNum();
	void generateWhileStmtNum();
	void generateIfStmtNum();
	void generateCallsStmtNum();
	void generateConstant();
	void generateStmtLstTable();
	void generateIfStmtLstTable();
	void generateProcedureConstant();

	void genereateFollows();
	void genereateFollowsStar();

	void genereateNextStar();

	vector<string> convertToStringResults(vector<int> _intResults);
};
