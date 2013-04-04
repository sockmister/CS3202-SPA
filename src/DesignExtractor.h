//Design Extractor traverses AST and compute all the remaining design abstractions: Uses, Modifies for procedure and call statement; Next; Affects
#pragma once

#include "PKB.h"
#include "CFG.h"
#include <stack>

class DesignExtractor{
public:
	//! Default constructor
	DesignExtractor();

	//! Constructor
	DesignExtractor(PKB* pkb);


	//! Build CFG of a procedure
	/*!
		\param STMT start - The first statement number of the procedure
		\param STMT end - The last statement number of the procedure
	*/
	//void buildCFG(STMT, STMT);
	void buildCFG(TNode, vector<std::pair<int,int>>*);

	//! Methods to compute design abstractions
	//! Note that computeCalls() must be called before computeModifiesAndUses()
	void computeCFG();
	string computeCalls(void);

	//! Compute Modifies and Uses for procedures and call statements
	void computeModifiesAndUses(void);

	//! Method to fill vector extra in ProcTable
	void fillExtra(void);

	//! Method to run optimisedCaller
	void computeOptimisedCaller(void);

private:
	AST * ast;
	VarTable * varTable;
	Follows * follows;
	Parent * parent;
	Modifies * modifies;
	Uses * uses;
	Calls * calls;
	ProcTable * procTable;
	OptimisedCaller * optimisedCaller;
	rootWhile * storeRootWhile;
	rootIf * storeRootIf;

	vector<STMTLST> Graph;
	vector<STMTLST> Block;

	bool flag; //flag = true if computeCalls() has been executed already
	vector<pair<pair<PROCNAME, PROCNAME>, STMT>> temp; //temp storage for call stmts

	int findFollows(int);
	int findLoopback(int);
	void setEndNode(vector<std::pair<int,int>> *);

};
