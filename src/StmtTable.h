#ifndef STMTTABLE_H
#define STMTTABLE_H

#include "Stmt.h";
#include <vector>

/*!  \class StmtTable
	 \brief A quick access table containing 'useful' information about a statement
*/

using namespace std;

class StmtTable {
private:
	vector<Stmt> * stmtTable;

public:
	StmtTable();
	~StmtTable();

	//! For Design Extractor's use only.
	void setCurrStmt (int nodeType, PROCNAME caller, PROCNAME callee);
	void setCurrStmt (int nodeType, PROCNAME caller, PROCNAME callee, string RHSExpr);
	
	//! get total number of statement
	/*!
		\return returns total number of statement
	*/
	int getSize();

	//! Get node type of statement at stmtNum.
	/*!
		\param int stmtNum
		\return returns the node type of the stmtNum
	*/
	int getNodeType (int stmtNum);
	
	//! Get procedure this statement belongs to.
	/*!
		\param int stmtNum
		\return returns PROCNAME of the procedure that stmtNum belongs to.
	*/
	PROCNAME getCaller (int stmtNum);
	
	//! If statement is Call, get callee (procedure it calls).
	/*!
		\param int stmtNum
		\return returns PROCNAME of the procedure is the callee
	*/
	PROCNAME getCallee (int stmtNum);
	
	//! If statement is Assign, get RHS expression.
	/*!
		\param int stmtNum
		\return returns the Right Side expression of a assignment statement
	*/
	string getAssignStmtRHSExpr (int stmtNum);
};
#endif