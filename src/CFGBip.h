#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "CFGLink.h"
#include "StmtTable.h"
#include "ProcTable.h"
#include <deque>

using namespace std;
typedef int STMT;
typedef vector<STMT> STMTLST;

/*!  \class CFGBip.
	 \brief Contains the CFGBip graph and methods for computing nextBip and nextBip*
*/


class CFGBip
{
public:
	//! Default constuctor
	CFGBip();
	//! Destructor
	~CFGBip();
	//! Constructor
	CFGBip(vector<vector<CFGLink>> * CFGBip, StmtTable * stmtTable, ProcTable * procTable);

   //! finds all possible control flow path from n
   /*!
	 \parm STMT program line number 
	 \parm STMT program line number 
	 \return an array of program line that has a control flow path from n
   */
	STMTLST nextBipStatements(STMT n, STMT branchFrom);

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

private:
	vector<bool> visited;
	bool skip, branch, call, first, exit;

	ProcTable * procTable;
	vector<vector<CFGLink>> * myCFGBip;
	StmtTable * stmtTable;
	STMT lastStmt, procedureFirstStmt, procedureLastStmt;
	STMT n1;
	STMT branchFrom;
	
	vector<bool> DFS(int,int); // Depth first search
	vector<bool> AffectsBipDFS(int,int); // Depth first search for AffectsBip*
	deque<STMT> callStack;

};

