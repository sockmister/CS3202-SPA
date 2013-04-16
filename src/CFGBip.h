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
	bool skip;

	ProcTable * procTable;
	vector<vector<CFGLink>> * myCFGBip;
	StmtTable * stmtTable;
	STMT lastStmt;
	STMT procedureFirstStmt;
	STMT procedureLastStmt;

	vector<bool> DFS(int); // Depth first search
	deque<STMT> callStack;

};

