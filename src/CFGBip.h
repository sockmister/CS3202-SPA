#pragma once
//#include "PKB.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "rootWhile.h"
#include "rootIf.h"
#include "CFGLink.h"
#include "ProcTable.h"
#include <deque>

using namespace std;
typedef int STMT;
typedef int ORDER;
typedef vector<STMT> STMTLST;

class CFGBip
{
public:
	//! Default constuctor
	CFGBip();
	//! Destructor
	~CFGBip();
	//! Constructor
	CFGBip(vector<vector<CFGLink>> CFGBip, ProcTable * procTable);
	
	//CFG(vector<STMTLST>, vector<STMTLST>);


   //! If order = 0, return statements that directly precedes programLine ,  If order = 1, return statements that appears directly after programLine
   /*!
	 \param order 0 or 1
	 \parm STMT program line number
	 \return otherwise empty STMTLST
   */
	STMTLST nextBipStatement (ORDER order, STMT programLine);

   //! If order = 0, return statements that precedes programLine ,  If order = 1, return statements that appears after programLine
   /*!
	 \param order 0 or 1
	 \parm STMT program line number
	 \return otherwise empty STMTLST
   */
	STMTLST nextBipStatementStar (ORDER order, STMT programLine);


   //! Check if n2 appears directly after n1
   /*!
	 \parm STMT program line number 1
	 \parm STMT program line number 2
	 \return TRUE if n2 appears directly after n1, otherwise return FALSE
   */
	bool isNextBip(STMT n1, STMT n2);

	//! Check if there is a control flow path from n1 to n2
   /*!
	 \parm STMT program line number 1
	 \parm STMT program line number 2
	 \return TRUE if there is a control flow path from n1 to n2, otherwise return FALSE
   */
	bool isNextBipStar(STMT n1, STMT n2);

	
	// For testing
	vector<std::pair<int,int>>  getCFG();

private:
	vector<bool> visited;
	bool skip;

	vector<vector<CFGLink>>  myCFGBip;
	//STMT firstStmt;
	STMT lastStmt;

	ProcTable * procTable;

	vector<bool> DFS(int); // Depth first search
	STMT procedureFirstStmt;
	STMT procedureLastStmt;

	deque<STMT> callStack;

	
};

